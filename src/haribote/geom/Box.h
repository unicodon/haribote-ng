#pragma once

#include "Geom.h"

class Box : public Geom {
public:
	Box(dReal lx, dReal ly, dReal lz, dSpaceID space);

	dGeom& geom() override { return m_geom; }
	void drawWireframe(const Camera& camera) override;

protected:
	dBox m_geom;
};
