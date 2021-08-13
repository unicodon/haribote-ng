#pragma once

#include "Body.h"

class Camera;

class Box {
protected:
	dBox m_geom;
	BodyPtr m_body;

public:
	Box() = delete;
	Box(dReal lx, dReal ly, dReal lz, dSpaceID space = 0) : m_geom(space, lx, ly, lz) { }

	virtual ~Box() = default;

	void setBody(BodyPtr);

	void drawWireframe(Camera&);
};
