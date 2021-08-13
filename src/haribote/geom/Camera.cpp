#include "Camera.h"

#include <math.h>
#include <ode/common.h>

Camera::Camera()
{
	position[0] = position[1] = position[2] = 0;

	m_tilt = -25 * M_PI / 180;
	m_pan = 0;
	m_roll = 0;

	zNear = 0.0100;
	zFar = 40.000;
	fov = 30;
	aspect = 1280.0/720.0;

	position[0] = 0.200;
	position[1] = -1.500;
	position[2] = 1.000;

	updateViewMatrix();
}

void Camera::setTilt(float rad)
{
	m_tilt = rad;
	updateViewMatrix();
}

void Camera::setPan(float rad)
{
	m_pan = rad;
	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	Matrix mat;
	mat.rotateZ(m_roll);
	mat.rotateX(m_tilt);
	mat.rotateY(m_pan);
	mat.rotateX(0.5 * M_PI);
	mat.translate(-position[0], -position[1], -position[2]);
	m_viewMatrix = mat;
}

Matrix Camera::view() const
{
	return m_viewMatrix;
}

Matrix Camera::projection() const
{
	float tx = tan(fov * aspect * M_PI / 180);
	float ty = tan(fov * M_PI / 180);
	return Matrix {
		1 / tx, 0, 0, 0,
		0, 1 / ty, 0, 0,
		0, 0, 1, -1,
		0, 0, 0, 0,
	};

	//return Matrix {
	//	0.2, 0, 0, 0,
	//	0, 0.5, 0, 0,
	//	0, 0, 0, 0,
	//	0, 0, 0.5, 1,
	//};
}
