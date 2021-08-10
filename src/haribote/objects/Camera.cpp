#include "Camera.h"

Camera::Camera()
{
	position[0] = position[1] = position[2] = 0;
	tilt = pan = roll = 0;

	zNear = 0.0100;
	zFar = 40.000;
	fov = 30;
	aspect = 1;

	position[0] = 0.000;
	position[1] = -1.500;
	position[2] = 0.800;
}