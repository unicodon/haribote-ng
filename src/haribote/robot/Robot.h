#pragma once

#include "../graphics/Light.h"
#include "../graphics/DrawMesh.h"
#include <ode/ode.h>

class Robot {
public:
	struct Part {
		Vector m_pos;
		std::vector<DrawMesh> m_drawMeshes;

		Part(const char* drawMeshPath, Vector pos);

		void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights);
	};

	void load(dWorldID world, dSpaceID space);

	void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights);

	std::vector<Part> parts;
};
