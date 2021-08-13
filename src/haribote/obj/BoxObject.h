#pragma once

#include "../geom/Box.h"

class BoxObject : public Box {
public:
	BoxObject(dReal weight, dReal lx, dReal ly, dReal lz, dWorldID world, dSpaceID space);

protected:
	dBody m_body;
};
