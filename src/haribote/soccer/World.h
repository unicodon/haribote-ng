#pragma once

#include <memory>

class World {
public:
	World() = default;
	virtual ~World() = default;

	virtual void draw() = 0;
	
	static std::unique_ptr<World> create();
};
