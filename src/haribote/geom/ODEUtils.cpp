#include "ODEUtils.h"

template<typename T>
Matrix getRotPos(const T& obj)
{
	const dReal* pos = obj.getPosition();
	const dReal* rot = obj.getRotation();
	return Matrix{
		(float)rot[0], (float)rot[4], (float)rot[8], 0,
		(float)rot[1], (float)rot[5], (float)rot[9], 0,
		(float)rot[2], (float)rot[6], (float)rot[10], 0,
		(float)pos[0], (float)pos[1], (float)pos[2], 1
	};
}

Matrix getRotPos(const dGeom& geom)
{
	return getRotPos<dGeom>(geom);
}

Matrix getRotPos(const dBody& body)
{
	return getRotPos<dBody>(body);
}
