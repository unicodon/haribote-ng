#pragma once

#include <ode/ode.h>
#include <vector>
#include "../util/Value.h"

struct TriMeshData {
	std::vector<int> indices;
	std::vector<dReal> vertices;
	std::vector<dReal> normals;
};

TriMeshData createTriMeshData(Value&& source);
