#include "World.h"

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../geom/Camera.h"
#include "../geom/GraphicsUtils.h"
#include "Field.h"
#include "Ball.h"

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
		m_world.setGravity(0, 0, -9.8);

		m_ball.setPosition(0, 0.1, 0.5);
	}

	~WorldImpl()
	{
	}

	void draw() override
	{
		dSpaceCollide(m_space, this, nearCallback);

		m_world.step(0.005);

		m_contactGroup.empty();

		static int rot = 0;
		rot++;
		m_mainCamera.setPan(0.2 * sin(rot * M_PI / 180));

		drawAxes(m_mainCamera, Matrix());

		m_field.draw(m_mainCamera);
		m_ball.drawWireframe(m_mainCamera);
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

	ODE m_ode;
	dWorld m_world;
	dHashSpace m_space;
	dJointGroup m_contactGroup;
	Field m_field;
	Ball m_ball;

	Camera m_mainCamera;
};

std::unique_ptr<World> World::create()
{
	return std::make_unique<WorldImpl>();
}
