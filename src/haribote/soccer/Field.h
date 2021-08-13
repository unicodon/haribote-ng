#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>
#include <memory>
class Camera;

class Field {
public:
	dWorld m_world;

public:
	Field(dWorldID, dSpaceID);
	~Field();

	void draw(Camera&);

private:
	dHashSpace m_space;
	dPlane m_ground;
};
