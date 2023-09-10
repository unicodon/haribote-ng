#pragma once

#include "../math/MatrixUtils.h"
#include "../geom/TriMeshData.h"
#include <ode/ode.h>
#include <ode/odecpp.h>
#include <GLES2/gl2.h>

class Camera;
struct LightInfo;

enum DrawFlags : unsigned {
	DrawGeom = 1u << 0,
	DrawGeomAxes = 1u << 1,
	DrawGeomWireframe = 1u << 2,
};

GLuint compileShader(GLuint shaderType, const GLchar* source);
GLuint wireframeProgram();

void drawAxes(const Camera& camera, const Matrix& transform);
void drawBoxWireframe(const Camera& camera, const Matrix& transform);
void drawSphereWireframe(const Camera& camera, const Matrix& transform);
void drawTriMeshWireframe(const Camera& camera, const Matrix& transform, const TriMeshData&);

void drawTriMesh(const Camera& camera, const LightInfo& lights, const Matrix& transform, const TriMeshData&);
