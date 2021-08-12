#include "Camera.h"

#include <math.h>
#include <ode/common.h>

Camera::Camera()
{
	position[0] = position[1] = position[2] = 0;
	tilt = pan = roll = 0;

	zNear = 0.0100;
	zFar = 40.000;
	fov = 30;
	aspect = 1;

	position[0] = 0.000;
	position[1] = -0.500;
	position[2] = 0.800;
}

Matrix<float> Camera::view() const
{
	Matrix<float> mat;
	mat.translate(-position[0], -position[1], -position[2]);
	return mat;
}

Matrix<float> Camera::projection() const
{
	//float tx = tan(fov * M_PI / 180);
	//float ty = tan(fov * aspect * M_PI / 180);
	//return Matrix<float> {
	//	1 / tx, 0, 0, 0,
	//	0, 1 / ty, 0, 0,
	//	0, 0, zFar / (zFar - zNear), - zFar * zNear / (zFar - zNear),
	//	0, 0, 1, 0,
	//};

	return Matrix<float> {
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, 0, 0,
		0, 0, 0.5, 1,
	};
}
