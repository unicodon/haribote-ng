#pragma once

#include <GLES2/gl2.h>

#include "../math/MatrixUtils.h"

class Camera {
protected:
	Vector<float> position;//視点
	GLfloat tilt, pan, roll;//向き

	GLfloat zNear, zFar;
	GLfloat fov;//視野角
	GLfloat aspect;//アスペクト比
public:
	Camera();
};
