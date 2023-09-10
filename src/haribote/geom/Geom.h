#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../graphics/Camera.h"
#include "../graphics/Light.h"
#include "../graphics/GraphicsUtils.h"

class Geom {
public:
	virtual ~Geom() = default;

	virtual dGeom& geom() = 0;
	virtual void draw(unsigned drawFlags, const Camera&, const LightInfo& lights) = 0;
};
