#include "World.h"

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../geom/Geom.h"
#include "../geom/GraphicsUtils.h"
#include "../obj/BoxObject.h"
#include "Field.h"
#include "Ball.h"

#include "../geom/MeshLoader.h"

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

const char* draw_mesh_filename[] = {
//	"mesh/draw_chin.x",
	"mesh/draw_body_tail.x",

	"mesh/draw_head_ear_eye.x",
	"mesh/draw_chin.x",
	"mesh/draw_neck.x",

	"mesh/draw_rf1.x",
	"mesh/draw_rf2.x",
	"mesh/draw_rf3.x",
	"mesh/draw_lf1.x",
	"mesh/draw_lf2.x",
	"mesh/draw_lf3.x",
	"mesh/draw_rh1.x",
	"mesh/draw_rh2.x",
	"mesh/draw_rh3.x",
	"mesh/draw_lh1.x",
	"mesh/draw_lh2.x",
	"mesh/draw_lh3.x",
};


const double ini_body_pos[][3] = {
	{	0.0,			0.0,		0.0},

	{	0.0675,			0.0,		0.0800 + 0.0195},
	{	0.0675 + 0.0400,	0.0,		0.0800 + 0.0195 - 0.0175},
	{	0.0675,			0.0,		0.0195},

	{	0.0650,			-0.125 / 2,	0},
	{	0.0650,			-0.125 / 2,	0},
	{	0.0650 + 0.0078,	-0.1344 / 2,	-0.0695},
	{	0.0650,			0.1250 / 2,	0},
	{	0.0650,			0.1250 / 2,	0},
	{	0.0650 + 0.0078,	0.1344 / 2,	-0.0695},
	{	-0.0650,		-0.1250 / 2,	0},
	{	-0.0650,		-0.1250 / 2,	0},
	{	-0.0728,		-0.1344 / 2,	-0.0695},
	{	-0.0650,		0.1250 / 2,	0},
	{	-0.0650,		0.1250 / 2,	0},
	{	-0.0728,		0.1344 / 2,	-0.0695},
};

const char* geom_mesh_filename[] = {
	"mesh/geom_body.x",

	"mesh/geom_head.x",
	"mesh/draw_chin.x",
	"mesh/geom_rf2.x",
	"mesh/geom_rf3.x",
	"mesh/geom_lf2.x",
	"mesh/geom_lf3.x",
	"mesh/geom_rh2.x",
	"mesh/geom_rh3.x",
	"mesh/geom_lh2.x",
	"mesh/geom_lh3.x",
};


struct Robot {
public:
	struct Part {
		Value mesh;
		Vector pos;
	};

	void load()
	{
		for (auto path : geom_mesh_filename) {
			loadMeshData(path);
		}

		for (int i = 0; i < 16; i++) {
			Part part;
			part.mesh = loadMeshData(draw_mesh_filename[i]);
			part.pos = Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]);
			parts.push_back(part);
		}

	}

	void draw(const Camera& camera)
	{
		static int rot = 0;
		rot++;
		Matrix m;
		m.scale(4);
		m.rotateZ(rot * 4 * M_PI / 180);
//		m.rotateZ(20 * M_PI / 180);
		for (auto& part : parts) {
			Matrix mat = m;
			mat.translate(part.pos);
			drawMesh(camera, part.mesh, mat);
		}

	}

	std::vector<Part> parts;
};

Robot robot;

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
		robot.load();

		m_world.setGravity(0, 0, -9.8);

		m_ball.geom().setPosition(0.2, 0, 0.5);

		auto box = std::make_unique<BoxObject>(1, 0.5, 0.5, 0.02, m_world, m_space);
		box->geom().setPosition(0, 0, 1);
		m_objects.push_back(std::move(box));
	}

	~WorldImpl()
	{
	}

	void draw() override
	{
		if (!m_paused) {
			dSpaceCollide(m_space, this, nearCallback);

			m_world.step(0.005);

			m_contactGroup.empty();
		}

		//static int rot = 0;
		//rot++;
		//m_mainCamera.setPan(0.2 * sin(rot * M_PI / 180));

//		drawAxes(m_mainCamera, Matrix());

//		m_field.draw(m_mainCamera);
//		m_ball.drawWireframe(m_mainCamera);

		for (auto& obj : m_objects) {
//			obj->drawWireframe(m_mainCamera);
		}

		robot.draw(m_mainCamera);
	}

	void pause() override
	{
		m_paused = !m_paused;
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

	bool m_paused = true;

	ODE m_ode;
	dWorld m_world;
	dHashSpace m_space;
	dJointGroup m_contactGroup;
	Field m_field;
	Ball m_ball;

	std::vector<std::unique_ptr<Geom>> m_objects;

	Camera m_mainCamera;
};

std::unique_ptr<World> World::create()
{
	return std::make_unique<WorldImpl>();
}
