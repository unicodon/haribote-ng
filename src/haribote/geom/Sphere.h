#pragma once

#include "Geom.h"

class Sphere : public Geom {
public:
	Sphere(dReal radius, dSpaceID space);

	dGeom& geom() override { return m_geom; }
	void draw(unsigned drawFlags, const Camera&, const LightInfo& lights) override;

protected:
	dSphere m_geom;
};
