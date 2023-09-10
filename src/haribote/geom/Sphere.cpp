#include "Sphere.h"

#include "../graphics/GraphicsUtils.h"
#include "ODEUtils.h"

Sphere::Sphere(dReal radius, dSpaceID space)
	: m_geom(space, radius)
{
}

void Sphere::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	if (drawFlags & DrawGeomAxes) {
		Matrix mat = getRotPos(m_geom);
		drawAxes(camera, mat);
	}

	if (drawFlags & DrawGeomWireframe) {
		dReal radius = m_geom.getRadius();

		Matrix mat = getRotPos(m_geom);

		mat.scale(radius);

		drawSphereWireframe(camera, mat);
	}
}
