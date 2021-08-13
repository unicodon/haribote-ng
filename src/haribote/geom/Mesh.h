#pragma once

#include "Geom.h"

class Mesh : public Geom {
public:
	Mesh(dSpaceID space);

	dGeom& geom() override { return m_geom; }
	void drawWireframe(const Camera& camera) override;

protected:
	dBox m_geom;
};
