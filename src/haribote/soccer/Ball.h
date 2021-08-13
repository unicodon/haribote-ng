#pragma once

#include "../geom/Sphere.h"

class Ball : public Sphere {
public:
	Ball(dWorldID, dSpaceID);

private:
	dBody m_body;
};
