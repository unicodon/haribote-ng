#include "Mesh.h"

#include "GraphicsUtils.h"
#include "ODEUtils.h"

void Mesh::draw(const Camera& camera, const LightInfo& lights)
{
	Matrix mat = getRotPos(m_geom);

	drawTriMesh(camera, lights, mat, m_data);
}


void Mesh::drawWireframe(const Camera& camera)
{
	Matrix mat = getRotPos(m_geom);

	drawAxes(camera, mat);

	drawTriMeshWireframe(camera, mat, m_data);
}
