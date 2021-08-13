#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>

class Camera;

class Ball {
public:
	Ball(dWorldID, dSpaceID);
	~Ball();

	void setPosition(dReal x, dReal y, dReal z);

	void drawWireframe(Camera&);
private:
	dSphere m_geom;
	dBody m_body;
};
