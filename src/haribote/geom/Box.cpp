#include "Box.h"

#include "Camera.h"
#include "GLES2/gl2.h"
#include <mutex>
#include "GraphicsUtils.h"

void Box::drawWireframe(Camera& camera)
{
	dVector3 length;
	m_geom.getLengths(length);

	Matrix mat;
	if (m_body) {
		auto pos = m_body->getPosition();
		for (int i = 0; i < 3; i++) {
			mat[3][i] = pos[i];
		}
	}
	else {
		auto pos = m_geom.getPosition();
		for (int i = 0; i < 3; i++) {
			mat[3][i] = pos[i];
		}
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
