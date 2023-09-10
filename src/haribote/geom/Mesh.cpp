#include "Mesh.h"

#include "../graphics/GraphicsUtils.h"
#include "ODEUtils.h"

void Mesh::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	Matrix mat = getRotPos(m_geom);
	
	if (drawFlags & DrawGeom) {
		drawTriMesh(camera, lights, mat, m_data);
	}
	if (drawFlags & DrawGeomAxes) {
		drawAxes(camera, mat);
	}
	if (drawFlags & DrawGeomWireframe) {
		drawTriMeshWireframe(camera, mat, m_data);
	}
}
