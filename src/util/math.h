#pragma once
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <random>
#include "../render/gui/Gui.h"

#define PI_F 3.141592654f
#define RAD_2_DEG 360.0f / (2 * PI_F)

namespace math
{
	glm::vec2 screenToWorld(glm::vec2 wdim, glm::vec2 coord, glm::mat4 ipvmat);

	//TODO: Convert this to output ivec2 to avoid problems with equalitites later on
	glm::vec2 snapToGrid(glm::vec2 location, float gridSX, float gridSY);

	bool isInsideRadius(glm::vec2 point, glm::vec2 center, float radius);

	inline std::string generateRandomID() {
		static std::random_device dev;
		static std::mt19937 rng(dev());

		std::uniform_int_distribution<int> dist(0, 15);

		const char* v = "0123456789abcdef";
		const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

		std::string res;
		for (int i = 0; i < 16; i++) {
			if (dash[i]) res += "-";
			res += v[dist(rng)];
			res += v[dist(rng)];
		}
		return res;
	}
}