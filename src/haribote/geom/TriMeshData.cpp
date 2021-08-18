#include "TriMeshData.h"
#include "../math/MatrixUtils.h"

static void calculateNormal(dReal* normal /* vec3 */, const dReal* vertices /* vec3 * 3 */)
{
	dReal v1[3] = {
		vertices[0] - vertices[6],
		vertices[1] - vertices[7],
		vertices[2] - vertices[8]
	};
	dReal v2[3] = {
		vertices[3] - vertices[6],
		vertices[4] - vertices[7],
		vertices[5] - vertices[8]
	};

	dReal prod[3] = {
		(v1[1] - v2[2]) * (v1[2] - v2[1]),
		(v1[2] - v2[0]) * (v1[0] - v2[2]),
		(v1[0] - v2[1]) * (v1[1] - v2[0])
	};

	dReal len = sqrt(prod[0] * prod[0] + prod[1] * prod[1] + prod[2] * prod[2]);
	if (len > 0) {
		normal[0] = prod[0] / len;
		normal[1] = prod[1] / len;
		normal[2] = prod[2] / len;
	}
	else {
		normal[0] = 1;
		normal[1] = 0;
		normal[2] = 0;
	}
}


TriMeshData createTriMeshData(Value&& source)
{
	auto& mesh = source.asObject();

	auto& vertices = mesh["vertices"].asArray();
	auto& faceVertices = mesh["faceVertices"].asArray();

	auto numTriangles = faceVertices.size();
	auto numVertices = vertices.size();

	TriMeshData data;
	data.indices.assign(numTriangles * 3, 0);
	data.vertices.assign(numVertices * 3, 0);
	data.normals.assign(numTriangles * 3, 0);

	for (size_t i = 0; i < numVertices; i++) {
		auto& v = vertices[i].asArray(); // [x, y , z]
		for (int j = 0; j < 3; j++) {
			data.vertices[i * 3 + j] = v[j].asNumber();
		}
	}

	for (size_t i = 0; i < numTriangles; i++) {
		auto& fv = faceVertices[i].asArray();
		data.indices[3 * i + 0] = fv[0].asInt();
		data.indices[3 * i + 1] = fv[1].asInt();
		data.indices[3 * i + 2] = fv[2].asInt();

		dReal vertices[9];
		for (int j = 0; j < 3; j++) {
			memcpy(vertices + 3 * j, data.vertices.data() + 3 * data.indices[3 * i + 0], 3 * sizeof(dReal));
		}

		calculateNormal(data.normals.data() + 3 * i, vertices);
	}

	return data;
}
