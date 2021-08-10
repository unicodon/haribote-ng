#include "World.h"

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../objects/Camera.h"
#include "Field.h"

dWorld g_world;
Field g_field;

Camera g_mainCamera;

void World::draw()
{
	g_field.draw(g_mainCamera);
}
