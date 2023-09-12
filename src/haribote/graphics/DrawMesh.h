#pragma once

#include "../util/Value.h"
#include "../graphics/Camera.h"
#include "../graphics/Light.h"
#include "../graphics/GraphicsUtils.h"
#include <vector>

class DrawMesh {
public:
	DrawMesh(Value&& source);

	void draw(unsigned drawFlags, const Matrix& transform, const Camera& camera, const LightInfo& lights);

private:
	std::vector<int> m_indices;
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
};