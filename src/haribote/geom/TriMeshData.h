#pragma once

#include <ode/ode.h>
#include <vector>

struct TriMeshData {
	std::vector<int> indices;
	std::vector<dReal> vertices;
	std::vector<float> normals;
};
