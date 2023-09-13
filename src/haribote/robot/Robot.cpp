#include "Robot.h"

#include "../geom/MeshLoader.h"
#include "../geom/ODEUtils.h"
#include "../geom/Sphere.h"
#include "../util/Json.h"

static Vector toVector(Value& value)
{
	auto& vec = value.asArray();
	return Vector(
		vec[0].asNumber(),
		vec[1].asNumber(),
		vec.size() > 2 ? vec[2].asNumber() : 0.0f,
		vec.size() > 3 ? vec[3].asNumber() : 1.0f
		);
}

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

Robot::Part::Part(dWorldID world, dSpaceID space, Value::Object& desc, const Matrix& transform)
	: m_body(world)
{
	auto massOffset = desc.find("massOffset");
	if (massOffset != desc.end()) {
		m_massOffset = toVector(desc["massOffset"]);
	}
	else {
		m_massOffset = Vector(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Vector offset = toVector(desc["offset"]);
	offset.translate(m_massOffset);
	offset = transform * offset;

	auto weight = desc.find("weight");
	if (weight != desc.end()) {
		const dReal Radius = 0.004;
		dMass mass;
		mass.setSphereTotal(weight->second.asNumber(), Radius);
		m_body.setMass(mass);
	}

	m_body.setPosition(offset[0], offset[1], offset[2]);

	auto geommesh = desc.find("geommesh");
	if (geommesh != desc.end()) {
		m_geom = std::make_unique<Sphere>(0.01, space);
		m_geom->geom().setBody(m_body);
	}

	auto drawmesh = desc.find("drawmesh");
	if (drawmesh != desc.end()) {
		auto drawmeshJson = parseJsonFile(drawmesh->second.asString().c_str());
		auto& meshData = drawmeshJson.asArray();
		for (auto& mesh : meshData) {
			m_drawMeshes.emplace_back(mesh);
		}
	}
}

void Robot::Part::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	static int rot = 0;
	rot++;

	Matrix m = getRotPos(m_body);
	m.translate(-m_massOffset[0], -m_massOffset[1], -m_massOffset[2]);
//	m.rotateZ(rot * 0.1 * M_PI / 180);

	//		m.rotateZ(20 * M_PI / 180);
	for (auto& drawMesh : m_drawMeshes) {
		//if (part.mesh) {
		//	part.mesh->draw(drawFlags, camera, lights);
		//}
		Matrix mat = m;
//		mat.translate(m_pos);
		//drawTriMesh(camera, lights, mat, part.drawMesh);
		drawMesh.draw(drawFlags, mat, camera, lights);
		//drawTriMesh(camera, mat, part.tri);
		//drawBoxWireframe(camera, mat);

	}

	{
		Matrix m = getRotPos(m_body);
		m.scale(0.005);

		GLfloat red[4] = { 1, 0, 0, 1 };

		drawSphereWireframe(camera, m, red);
	}

	if (m_geom) {
		m_geom->draw(drawFlags, camera, lights);
	}
}

Robot::Joint::Joint(dWorldID world, dBody& body0, dBody& body1, Value::Object& desc, const Matrix& transform) : m_joint(world)
{
	m_joint.attach(body0, body1);

	Vector axis = transform * toVector(desc["axis"]);
	m_joint.setAxis(axis[0], axis[1], axis[2]);

	Vector anchor = transform * toVector(desc["anchor"]);
	m_joint.setAnchor(anchor[0], anchor[1], anchor[2]);

	Vector range = toVector(desc["angle_range"]);
	m_joint.setParam(dParamLoStop, range[0] * M_PI / 180);
	m_joint.setParam(dParamHiStop, range[1] * M_PI / 180);

}

void Robot::Joint::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	{
		dReal anchor[3];
		m_joint.getAnchor(anchor);
		Matrix m;
		m.translate(anchor[0], anchor[1], anchor[2]);
		m.scale(0.002);

		GLfloat yellow[4] = { 1, 1, 0, 1 };

		drawSphereWireframe(camera, m, yellow);
	}
}


void Robot::load(dWorldID world, dSpaceID space, const Matrix& transform)
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

	Value model = parseJsonFile("data/model.json");
	auto& parts = model.asObject()["parts"].asObject();
	for (auto it : parts) {
		Value::Object& desc = it.second.asObject();
		m_parts[it.first] = std::make_unique<Part>(world, space, desc, transform);
	}

	auto joints = model.asObject()["joints"].asObject();
	for (auto it : joints) {
		Value::Object& desc = it.second.asObject();
		auto& connectedParts = desc["connect"].asArray();
		auto parts0 = m_parts.find(connectedParts[0].asString());
		auto parts1 = m_parts.find(connectedParts[1].asString());
		m_joints[it.first] = std::make_unique<Joint>(world, parts0->second->body(), parts1->second->body(), desc, transform);
	}

	//int i = 0;
	//parts.reserve(16);
	//for (auto path : new_mesh_filename) {
	//	parts.emplace_back(path, Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));
	//	i++;
	//}


	//for (int i = 0; i < 16; i++) {
	//	parts.emplace_back(DrawMesh(loadMeshData(draw_mesh_filename[i])), Vector(ini_body_pos[i][0], ini_body_pos[i][1], ini_body_pos[i][2]));
	//}

}

void Robot::draw(unsigned drawFlags, const Camera& camera, const LightInfo& lights)
{
	for (auto& it : m_parts) {
		//if (part.mesh) {
		//	part.mesh->draw(drawFlags, camera, lights);
		//}
		//drawTriMesh(camera, lights, mat, part.drawMesh);
		it.second->draw(drawFlags, camera, lights);
		//drawTriMesh(camera, mat, part.tri);
		//drawBoxWireframe(camera, mat);

	}

	for (auto& it : m_joints) {
		it.second->draw(drawFlags, camera, lights);
	}
}
