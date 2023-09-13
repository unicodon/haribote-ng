#pragma once

#include "../graphics/Light.h"
#include "../graphics/DrawMesh.h"
#include "../geom/Geom.h"
#include <ode/ode.h>
#include <memory>

class Robot {
public:
	class Part {
	public:
		Part(dWorldID world, dSpaceID space, Value::Object& desc, const Matrix& transform);

		void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights);

		dBody& body() { return m_body; }
	private:
		dBody m_body;
		Vector m_massOffset;
		std::unique_ptr<Geom> m_geom;
		std::vector<DrawMesh> m_drawMeshes;
	};
	using PartPtr = std::unique_ptr<Part>;

	class Joint {
	public:
		Joint(dWorldID world, dBody& body0, dBody& body1, Value::Object& desc, const Matrix& transform);

		void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights);

	private:
		dHingeJoint m_joint;
	};
	using JointPtr = std::unique_ptr<Joint>;

	void load(dWorldID world, dSpaceID space, const Matrix& transform);

	void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights);

private:
	std::map<std::string, PartPtr> m_parts;
	std::map<std::string, JointPtr> m_joints;
};
