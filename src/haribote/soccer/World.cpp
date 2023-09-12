#include "World.h"

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../geom/Geom.h"
#include "../graphics/GraphicsUtils.h"
#include "../obj/BoxObject.h"
#include "Field.h"
#include "Ball.h"
#include "../geom/TriMeshData.h"
#include "../geom/Mesh.h"

#include "../graphics/Light.h"
#include "../graphics/DrawMesh.h"

#include "../robot/Robot.h"

#include <memory>
#include <vector>

class ODE {
public:
	ODE()
	{
		dInitODE();
	}

	~ODE()
	{
		dCloseODE();
	}
};

Robot* robot;

class WorldImpl : public World
{
public:
	WorldImpl()
		: m_ode()
		, m_world()
		, m_space()
		, m_contactGroup()
		, m_field(m_world, m_space)
		, m_ball(m_world, m_space)
	{
		robot = new Robot;
		robot->load(m_world, m_space);

		m_world.setGravity(0, 0, -9.8);

		m_ball.geom().setPosition(0.2, 0, 0.5);

		auto box = std::make_unique<BoxObject>(1, 0.5, 0.5, 0.02, m_world, m_space);
		box->geom().setPosition(0, 0, 1);
		m_objects.push_back(std::move(box));

		m_mainCamera.setPosition(0, -0.3, 0.2);
		m_mainCamera.lookAt(0, 0, 0);

		initializeMainLights();
	}

	~WorldImpl()
	{
		delete robot;
	}

	void draw() override
	{
		if (!m_paused) {
			dSpaceCollide(m_space, this, nearCallback);

			m_world.step(0.002);

			m_contactGroup.empty();
		}

		//static int rot = 0;
		//rot++;
		//m_mainCamera.setPan(0.2 * sin(rot * M_PI / 180));

		drawAxes(m_mainCamera, Matrix());

		unsigned drawFlags = DrawGeomAxes | DrawGeomWireframe;

		m_field.draw(m_mainCamera);
		m_ball.draw(drawFlags, m_mainCamera, m_mainLights);

		for (auto& obj : m_objects) {
			obj->draw(drawFlags, m_mainCamera, m_mainLights);
		}

		robot->draw(drawFlags, m_mainCamera, m_mainLights);
	}

	void pause() override
	{
		m_paused = !m_paused;
	}

	void tilt(float rad) override
	{
		m_mainCamera.setTilt(m_mainCamera.getTilt() + rad);
	}

	void pan(float rad) override
	{
		m_mainCamera.setPan(m_mainCamera.getPan() + rad);
	}


	void initializeMainLights()
	{
		{
			DirectionalLight light;
			light.direction = Vector(0, 0, -1);
			light.ambient = Vector(0.1, 0.1, 0.1);
			light.diffuse = Vector(0.3, 0.3, 0.3);
			light.specular = Vector(0, 0, 0);
			m_mainLights.directionalLights[0] = std::move(light);
		}
	}

private:
	static void nearCallback(void* data, dGeomID o1, dGeomID o2)
	{
		WorldImpl* that = (WorldImpl*)data;
		if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
		{
			dSpaceCollide2(o1, o2, data, &nearCallback);

			if (dGeomIsSpace(o1)) dSpaceCollide((dSpaceID)o1, data, &nearCallback);
			if (dGeomIsSpace(o2)) dSpaceCollide((dSpaceID)o2, data, &nearCallback);
		}
		else
		{
			if (dGeomGetSpace(o1) == dGeomGetSpace(o2) && dGeomGetSpace(o1) != that->m_space.id())
			{
				return;
			}

			int i, n;

			const int N = 30;
			dContact contact[N];
			n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
			if (n > 0) {
				for (i = 0; i < n; i++) {
					contact[i].surface.mode = dContactApprox1 | dContactBounce;
					contact[i].surface.mu = 3.0e-1;
					contact[i].surface.mu2 = 3.0e-1;
					contact[i].surface.slip1 = 0.3;
					contact[i].surface.slip2 = 0.3;
					contact[i].surface.soft_erp = 0.4;
					contact[i].surface.soft_cfm = 1.0e-2;
					contact[i].surface.bounce = 0.2;
					dJointID c = dJointCreateContact(that->m_world, that->m_contactGroup, &contact[i]);
					dBodyID b1 = dGeomGetBody(o1);
					dBodyID b2 = dGeomGetBody(o2);
					dJointAttach(c, b1, b2);
				}
			}
		}
	}

	bool m_paused = false;

	ODE m_ode;
	dWorld m_world;
	dHashSpace m_space;
	dJointGroup m_contactGroup;
	Field m_field;
	Ball m_ball;

	std::vector<std::unique_ptr<Geom>> m_objects;

	Camera m_mainCamera;

	LightInfo m_mainLights;
};

std::unique_ptr<World> World::create()
{
	return std::make_unique<WorldImpl>();
}
