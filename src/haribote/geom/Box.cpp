#include "Box.h"

#include "../graphics/GraphicsUtils.h"
#include "ODEUtils.h"

Box::Box(dReal lx, dReal ly, dReal lz, dSpaceID space)
	: m_geom(space, lx, ly, lz)
{
}

void Box::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	if (drawFlags & DrawGeomAxes) {
		Matrix mat = getRotPos(m_geom);
		drawAxes(camera, mat);
	}

	if (drawFlags & DrawGeomWireframe) {
		dVector3 length;
		m_geom.getLengths(length);

		Matrix mat = getRotPos(m_geom);

		mat.scaleX(length[0]);
		mat.scaleY(length[1]);
		mat.scaleZ(length[2]);

		drawBoxWireframe(camera, mat);
	}
}
