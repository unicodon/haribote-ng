#include "BoxObject.h"

BoxObject::BoxObject(dReal weight, dReal lx, dReal ly, dReal lz, dWorldID world, dSpaceID space)
	: Box(lx, ly, lz, space)
	, m_body(world)
{
	dMass mass;
	mass.setBoxTotal(weight, lx, ly, lz);
	m_body.setMass(mass);
	m_geom.setBody(m_body);
}
