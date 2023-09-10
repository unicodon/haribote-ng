#pragma once

#include "Geom.h"
#include "TriMeshData.h"
#include <ode/odecpp_collision.h>

class dTriMesh : public dGeom {
    // intentionally undefined, don't use these
    dTriMesh(dTriMesh&);
    void operator= (dTriMesh&);

    dTriMeshDataID m_triMeshData;

public:
    dTriMesh(dSpaceID space, const TriMeshData& data)
    {
        m_triMeshData = dGeomTriMeshDataCreate();
#if defined(dSINGLE)
#define dGeomTriMeshDataBuildReal dGeomTriMeshDataBuildSingle
#else
#define dGeomTriMeshDataBuildReal dGeomTriMeshDataBuildDouble
#endif
        dGeomTriMeshDataBuildReal(m_triMeshData, data.vertices.data(), 3 * sizeof(dReal), data.vertices.size() / 3, data.indices.data(), data.indices.size(), 3 * sizeof(int));
        _id = dCreateTriMesh(space, m_triMeshData, nullptr, nullptr, nullptr);
    }

    ~dTriMesh()
    {
        dGeomTriMeshDataDestroy(m_triMeshData);
    }
};

class Mesh : public Geom {
public:
    Mesh(dSpaceID space, TriMeshData&& data) : m_geom(space, data), m_data(std::move(data)) { }

	dGeom& geom() override { return m_geom; }
    
    void draw(unsigned drawFlags, const Camera&, const LightInfo& lights) override;

protected:
    dTriMesh m_geom;
    TriMeshData m_data;
};
