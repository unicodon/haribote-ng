#include "Field.h"

#include "../objects/Box.h"

void Field::draw(Camera& camera)
{
	m_ground->drawWireframe(camera);
}


Field::Field()
{
	m_ground = std::make_unique<Box>(10, 10, 10);
}