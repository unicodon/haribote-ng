#pragma once

#include "../math/MatrixUtils.h"
#include <ode/ode.h>
#include <ode/odecpp.h>
#include <GLES2/gl2.h>

class Camera;

GLuint compileShader(GLuint shaderType, const GLchar* source);

void drawAxes(const Camera& camera, const Matrix& transform);
void drawBoxWireframe(const Camera& camera, const Matrix& transform);
void drawSphereWireframe(const Camera& camera, const Matrix& transform);
