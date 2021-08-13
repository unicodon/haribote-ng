#include "Box.h"

#include "Camera.h"
#include "GLES2/gl2.h"
#include <mutex>
#include "GraphicsUtils.h"
#include "ODEUtils.h"

void Box::drawWireframe(Camera& camera)
{
	dVector3 length;
	m_geom.getLengths(length);

	Matrix mat;
	if (m_body) {
		mat = getRotPos(*m_body);
	}
	else {
		mat = getRotPos(m_geom);
	}

	drawAxes(camera, mat);

	mat.scaleX(length[0]);
	mat.scaleY(length[1]);
	mat.scaleZ(length[2]);

	drawBoxWireframe(camera, mat);
}


void Box::setBody(BodyPtr body)
{
	m_body = std::move(body);
	m_geom.setBody(m_body->id());
}
