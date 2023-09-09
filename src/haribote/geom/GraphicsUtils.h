#pragma once

#include "../math/MatrixUtils.h"
#include "TriMeshData.h"
#include <ode/ode.h>
#include <ode/odecpp.h>
#include <GLES2/gl2.h>

class Camera;
struct LightInfo;

GLuint compileShader(GLuint shaderType, const GLchar* source);
GLuint wireframeProgram();

void drawAxes(const Camera& camera, const Matrix& transform);
void drawBoxWireframe(const Camera& camera, const Matrix& transform);
void drawSphereWireframe(const Camera& camera, const Matrix& transform);
void drawTriMeshWireframe(const Camera& camera, const Matrix& transform, const TriMeshData&);

void drawTriMesh(const Camera& camera, const LightInfo& lights, const Matrix& transform, const TriMeshData&);
