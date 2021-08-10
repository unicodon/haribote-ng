#pragma once

#include "Body.h"

class Box : public Body {
protected:
	// Box size
	dReal m_cx = 0;
	dReal m_cy = 0;
	dReal m_cz = 0;

public:
	Box() = default;
	Box(dReal cx, dReal cy, dReal cz) : m_cx(cx), m_cy(cy), m_cz(cz) { }
	virtual ~Box() = default;
	void drawWireframe(Camera&) override;
};
