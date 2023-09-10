#include "Ball.h"

#include "../graphics/GraphicsUtils.h"
#include "../geom/ODEUtils.h"

const dReal BallRadius = 0.04;
const dReal BallWeight = 0.01;

Ball::Ball(dWorldID world, dSpaceID space)
	: Sphere(BallRadius, space)
	, m_body(world)
{
	dMass mass;
	mass.setSphereTotal(BallWeight, BallRadius);
	m_body.setMass(mass);
	m_geom.setBody(m_body);
}
