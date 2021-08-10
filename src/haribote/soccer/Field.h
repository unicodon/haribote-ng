#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>
#include <memory>
#include "../objects/Box.h"
class Camera;

class Field {
public:
	dWorld m_world;

public:
	Field();

	void draw(Camera&);

private:
	std::unique_ptr<Box> m_ground;
};
