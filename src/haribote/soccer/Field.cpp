#include "Field.h"

#include "../geom/Box.h"


Field::Field(dWorldID world, dSpaceID worldSpace)
	: m_space(worldSpace)
	, m_ground(m_space, 0, 0, 1, 0) // A plane geom where z = 0.
{
	m_box = std::make_unique<Box>(0.5, 0.5, 0.02, worldSpace);
	BodyPtr body = std::make_unique<dBody>(world);
	body->setPosition(0, 0, 0.8);
	dMass mass;
	mass.setBoxTotal(1, 0.5, 0.5, 0.02);
	body->setMass(mass);
	m_box->setBody(std::move(body));
}

Field::~Field()
{
}

void Field::draw(Camera& camera)
{
	m_box->drawWireframe(camera);
}

