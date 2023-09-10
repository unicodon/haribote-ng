#pragma once

#include <vector>
#include "../graphics/Camera.h"
#include "../util/Value.h"

using MeshData = void*;

void drawMesh(const Camera& camera, Value& data, const Matrix& transform);

Value loadJsonMeshData(const char* path);

Value loadMeshData(const char* path);
MeshData parseMeshData(const std::vector<char>& data);

void exportMeshData(const char* path, MeshData mesh);
