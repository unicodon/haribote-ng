#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>

class Camera;

class Body {
protected:
	dReal position[16];
	dSpaceID m_space;
	dWorldID m_world;

public:
	virtual ~Body() = default;
	virtual void drawWireframe(Camera&) = 0;
};
