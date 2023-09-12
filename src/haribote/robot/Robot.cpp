#include "Robot.h"

#include "../geom/MeshLoader.h"

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

const double ini_body_pos[][3] = {
	{	0.0,			0.0,		0.0},

	{	0.0675,			0.0,		0.0800 + 0.0195},
	{	0.0675 + 0.0400,	0.0,		0.0800 + 0.0195 - 0.0175},
	{	0.0675,			0.0,		0.0195},

	{	0.0650,			0 / 2,	0},
	{	0.0650,			-0.125 / 2,	0},
	{	0.0650 + 0.0078,	-0.1344 / 2,	-0.0695},

	{	0.0650,			0 / 2,	0},
	{	0.0650,			0.1250 / 2,	0},
	{	0.0650 + 0.0078,	0.1344 / 2,	-0.0695},

	{	-0.0650,		0 / 2,	0},
	{	-0.0650,		-0.1250 / 2,	0},
	{	-0.0728,		-0.1344 / 2,	-0.0695},

	{	-0.0650,		0 / 2,	0},
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

const char* new_mesh_filename[] = {
	"newmesh/draw_body.json",
	"newmesh/draw_head.json",
	"newmesh/draw_chops.json",
	"newmesh/draw_neck.json",

	"newmesh/draw_rf1.json",
	"newmesh/draw_rf2.json",
	"newmesh/draw_rf3.json",

	"newmesh/draw_lf1.json",
	"newmesh/draw_lf2.json",
	"newmesh/draw_lf3.json",

	"newmesh/draw_rh1.json",
	"newmesh/draw_rh2.json",
	"newmesh/draw_rh3.json",

	"newmesh/draw_lh1.json",
	"newmesh/draw_lh2.json",
	"newmesh/draw_lh3.json",
};

Robot::Part::Part(const char* drawMeshPath, Vector pos)
	: m_pos(pos)
{
	auto meshData = loadJsonMeshData(drawMeshPath).asArray();
	for (auto& mesh : meshData) {
		m_drawMeshes.emplace_back(std::move(mesh));
	}
}

void Robot::Part::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	static int rot = 0;
	rot++;
	Matrix m;
	m.scale(1);
	m.rotateZ(rot * 0.1 * M_PI / 180);
	//		m.rotateZ(20 * M_PI / 180);
	for (auto& drawMesh : m_drawMeshes) {
		//if (part.mesh) {
		//	part.mesh->draw(drawFlags, camera, lights);
		//}
		Matrix mat = m;
		mat.translate(m_pos);
		//drawTriMesh(camera, lights, mat, part.drawMesh);
		drawMesh.draw(drawFlags, mat, camera, lights);
		//drawTriMesh(camera, mat, part.tri);
		//drawBoxWireframe(camera, mat);

	}
}


void Robot::load(dWorldID world, dSpaceID space)
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
	parts.reserve(16);
	for (auto path : new_mesh_filename) {
		parts.emplace_back(path, Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));
		i++;
	}


	//for (int i = 0; i < 16; i++) {
	//	parts.emplace_back(DrawMesh(loadMeshData(draw_mesh_filename[i])), Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));
	//}

}

void Robot::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	for (auto& part : parts) {
		//if (part.mesh) {
		//	part.mesh->draw(drawFlags, camera, lights);
		//}
		//drawTriMesh(camera, lights, mat, part.drawMesh);
		part.draw(drawFlags, camera, lights);
		//drawTriMesh(camera, mat, part.tri);
		//drawBoxWireframe(camera, mat);

	}

}
