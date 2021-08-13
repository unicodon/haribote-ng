#include "Ball.h"

#include "../geom/GraphicsUtils.h"
#include "../geom/ODEUtils.h"

const dReal BallRadius = 0.1;
const dReal BallWeight = 0.01;

Ball::Ball(dWorldID world, dSpaceID space)
	: m_geom(space, BallRadius)
	, m_body(world)
{
	dMass mass;
	mass.setSphereTotal(BallWeight, BallRadius);
	m_body.setMass(mass);
	m_geom.setBody(m_body);
}

Ball::~Ball()
{
}

void Ball::setPosition(dReal x, dReal y, dReal z)
{
	m_body.setPosition(x, y, z);
}

void Ball::drawWireframe(Camera& camera)
{
	dReal radius = m_geom.getRadius();

	Matrix mat = getRotPos(m_body);

	drawAxes(camera, mat);

	mat.scale(radius);

	drawSphereWireframe(camera, mat);
}
