#pragma once

#include "Geom.h"

class Box : public Geom {
public:
	Box(dReal lx, dReal ly, dReal lz, dSpaceID space);

	dGeom& geom() override { return m_geom; }
	void draw(unsigned drawFlags, const Camera&, const LightInfo& lights) override;

protected:
	dBox m_geom;
};
