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

#include "../geom/MeshLoader.h"
#include "../graphics/Light.h"
#include "../graphics/DrawMesh.h"

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

const char* new_mesh[] = {
	"newmesh/out.json"
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
//	"mesh/draw_body_tail.x",
//	"mesh/draw_head_ear_eye.x",
//	"mesh/draw_chin.x",

	"mesh/geom_body.x",

	//"mesh/geom_head.x",
	//"mesh/draw_chin.x",
	//"mesh/geom_rf2.x",
	//"mesh/geom_rf3.x",
	//"mesh/geom_lf2.x",
	//"mesh/geom_lf3.x",
	//"mesh/geom_rh2.x",
	//"mesh/geom_rh3.x",
	//"mesh/geom_lh2.x",
	//"mesh/geom_lh3.x",
};


struct Robot {
public:
	struct Part {
//		std::unique_ptr<Mesh> mesh;
		DrawMesh drawMesh;
		Vector pos;

		Part(DrawMesh&& drawMesh, Vector&& pos) : drawMesh(std::move(drawMesh)), pos(std::move(pos)) {}
	};

	void load(dWorldID world, dSpaceID space)
	{
		//for (auto path : geom_mesh_filename) {
		//	Part part;
		//	auto mesh = createTriMeshData(loadMeshData(path));
		//	part.mesh = std::make_unique<Mesh>(space, std::move(mesh));

		//	dBody* b = new dBody(world);
		//	dMass mass;
		//	mass.setBoxTotal(1, 0.1, 0.1, 0.1);
		//	b->setMass(mass);
		//	part.mesh->geom().setBody(*b);
		//	part.mesh->geom().setPosition(0, 0, .5);

		//	parts.push_back(std::move(part));
		//	return;
		//}

		int i = 0;
		parts.emplace_back(DrawMesh(std::move(loadJsonMeshData("newmesh/out.json").asArray()[0])), Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));

		//for (int i = 0; i < 16; i++) {
		//	parts.emplace_back(DrawMesh(loadMeshData(draw_mesh_filename[i])), Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));
		//}

	}

	void draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
	{
		static int rot = 0;
		rot++;
		Matrix m;
		m.scale(1);
		m.rotateZ(rot * 1 * M_PI / 180);
//		m.rotateZ(20 * M_PI / 180);
		for (auto& part : parts) {
			//if (part.mesh) {
			//	part.mesh->draw(drawFlags, camera, lights);
			//}
			Matrix mat = m;
			mat.translate(part.pos);
			//drawTriMesh(camera, lights, mat, part.drawMesh);
			part.drawMesh.draw(drawFlags, mat, camera, lights);
			//drawTriMesh(camera, mat, part.tri);
			//drawBoxWireframe(camera, mat);

		}

	}

	std::vector<Part> parts;
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
