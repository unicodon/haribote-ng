#pragma once

#include "Geom.h"
#include "TriMeshData.h"
#include <ode/odecpp_collision.h>

class dTriMesh : public dGeom {
    // intentionally undefined, don't use these
    dTriMesh(dTriMesh&);
    void operator= (dTriMesh&);

public:
    dTriMesh(dSpaceID space, const TriMeshData& data)
    {
        dTriMeshDataID g = dGeomTriMeshDataCreate();
#if defined(dSINGLE)
        dGeomTriMeshDataBuildSingle(g, data.vertices.data(), 3 * sizeof(dReal), data.vertices.size() / 3, data.indices.data(), data.indices.size() / 3, 3 * sizeof(int));
#else
        dGeomTriMeshDataBuildDouble1(g, data.vertices.data(), 3 * sizeof(dReal), data.vertices.size() / 3, data.indices.data(), data.indices.size() / 3, 3 * sizeof(int), data.normals.data());
#endif
        _id = dCreateTriMesh(space, g, nullptr, nullptr, nullptr);
//        dGeomTriMeshDataDestroy(g);
    }
};

class Mesh : public Geom {
public:
    Mesh(dSpaceID space, TriMeshData&& data) : m_geom(space, data), m_data(std::move(data)) { }

	dGeom& geom() override { return m_geom; }
    
    void draw(const Camera&, const LightInfo& lights) override;
    void drawWireframe(const Camera& camera) override;

protected:
    dTriMesh m_geom;
    TriMeshData m_data;
};
