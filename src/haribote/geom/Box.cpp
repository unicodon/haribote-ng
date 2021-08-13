#include "Box.h"

#include "GraphicsUtils.h"
#include "ODEUtils.h"

Box::Box(dReal lx, dReal ly, dReal lz, dSpaceID space)
	: m_geom(space, lx, ly, lz)
{
}

void Box::drawWireframe(const Camera& camera)
{
	dVector3 length;
	m_geom.getLengths(length);

	Matrix mat = getRotPos(m_geom);

	drawAxes(camera, mat);

	mat.scaleX(length[0]);
	mat.scaleY(length[1]);
	mat.scaleZ(length[2]);

	drawBoxWireframe(camera, mat);
}
