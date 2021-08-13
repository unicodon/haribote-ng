#include "Field.h"

Field::Field(dWorldID world, dSpaceID worldSpace)
	: m_space(worldSpace)
	, m_ground(m_space, 0, 0, 1, 0) // A plane geom where z = 0.
{
}

Field::~Field()
{
}

void Field::draw(Camera& camera)
{
}

