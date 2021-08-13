#include "Sphere.h"

#include "GraphicsUtils.h"
#include "ODEUtils.h"

Sphere::Sphere(dReal radius, dSpaceID space)
	: m_geom(space, radius)
{
}

void Sphere::drawWireframe(const Camera& camera)
{
	dReal radius = m_geom.getRadius();

	Matrix mat = getRotPos(m_geom);

	drawAxes(camera, mat);

	mat.scale(radius);

	drawSphereWireframe(camera, mat);
}
