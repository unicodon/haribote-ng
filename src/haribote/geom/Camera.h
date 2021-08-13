#pragma once

#include <GLES2/gl2.h>

#include "../math/MatrixUtils.h"

class Camera {
public:
	Camera();

	Matrix view() const;
	Matrix projection() const;

	void setTilt(float rad);
	void setPan(float rad);

protected:
	void updateViewMatrix();

	Vector position;//視点
	float m_tilt;
	float m_pan;
	float m_roll;

	GLfloat zNear, zFar;
	GLfloat fov;//視野角
	GLfloat aspect;//アスペクト比

	Matrix m_viewMatrix;
};
