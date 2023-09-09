// app.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "app.h"

#include <vector>
#include <memory>
#include "EGL/egl.h"
#include "GLES2/gl2.h"

#include "soccer/world.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

class Display {
public:
    EGLDisplay m_display = EGL_NO_DISPLAY;
    Display()
    {
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        EGLint major, minor;
        eglInitialize(m_display, &major, &minor);
    }

    ~Display()
    {
        eglTerminate(m_display);
    }
};

EGLDisplay display()
{
    static Display g_disp;
    return g_disp.m_display;
}

EGLConfig config()
{
    std::vector<EGLint> attr;
    attr.push_back(EGL_RED_SIZE);
    attr.push_back(8);
    attr.push_back(EGL_GREEN_SIZE);
    attr.push_back(8);
    attr.push_back(EGL_BLUE_SIZE);
    attr.push_back(8);
    attr.push_back(EGL_RENDERABLE_TYPE);
    attr.push_back(EGL_OPENGL_ES2_BIT);
    attr.push_back(EGL_SURFACE_TYPE);
    attr.push_back(EGL_WINDOW_BIT);
    attr.push_back(EGL_NONE);

    EGLConfig config;
    EGLint numConfig;
    eglChooseConfig(display(), attr.data(), &config, 1, &numConfig);
    return config;
}

class WindowSurface {
public:
    EGLSurface m_surface = EGL_NO_SURFACE;

    WindowSurface(HWND hWnd)
    {
        m_surface = eglCreateWindowSurface(display(), config(), hWnd, NULL);
    }

    ~WindowSurface()
    {
        eglDestroySurface(display(), m_surface);
    }
};

std::unique_ptr<WindowSurface> g_surface;

class Context {
public:
    EGLContext m_context;
    Context()
    {
        std::vector<EGLint> attr;
        attr.push_back(EGL_CONTEXT_CLIENT_VERSION);
        attr.push_back(2);
        attr.push_back(EGL_NONE);
        m_context = eglCreateContext(display(), config(), EGL_NO_CONTEXT, attr.data());
    }

    ~Context()
    {
        eglDestroyContext(display(), m_context);
    }
};

std::unique_ptr<Context> g_context;

std::unique_ptr<World> g_world;


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT rect;
   rect.left = 0;
   rect.top = 0;
   rect.right = 1280;
   rect.bottom = 720;
   AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_world = World::create();

   g_surface = std::make_unique<WindowSurface>(hWnd);
   g_context = std::make_unique<Context>();

   eglMakeCurrent(display(), g_surface->m_surface, g_surface->m_surface, g_context->m_context);

   SetTimer(hWnd, 1, 20, nullptr);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
        {
           //g_surface = std::make_unique<WindowSurface>(hWnd);

           //eglMakeCurrent(display(), g_surface->m_surface, g_surface->m_surface, g_context->m_context);
        }
        break;
    case WM_TIMER:
        {
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            if (g_world)
                g_world->draw();

            glFlush();
            eglSwapBuffers(display(), g_surface->m_surface);
        }
        break;
    case WM_CHAR:
        switch (wParam) {
        case 'p':
            g_world->pause();
            break;
        case 'j':
            g_world->pan(0.2f);
            break;
        case 'k':
            g_world->pan(-0.2f);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        g_world.reset();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
