#pragma once

#include "../math/MatrixUtils.h"
#include <array>
#include <optional>

struct DirectionalLight {
	Vector direction = Vector(0, 0, -1);
	Vector ambient = Vector(0, 0, -1);
	Vector diffuse = Vector(0, 0, -1);
	Vector specular = Vector(0, 0, -1);
};

struct LightInfo {
	static constexpr int MaxDirectionalLightCount = 3;
	std::array<std::optional<DirectionalLight>, MaxDirectionalLightCount> directionalLights;
};
