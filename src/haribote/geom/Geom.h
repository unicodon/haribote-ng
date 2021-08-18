#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "Camera.h"
#include "Light.h"

class Geom {
public:
	virtual ~Geom() = default;

	virtual dGeom& geom() = 0;
	virtual void draw(const Camera&, const LightInfo& lights) {};
	virtual void drawWireframe(const Camera&) = 0;
};
