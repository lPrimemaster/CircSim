#pragma once
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include "../render/gui/Gui.h"

#define PI_F 3.141592654f

namespace math
{
	glm::vec2 screenToWorld(glm::vec2 wdim, glm::vec2 coord, glm::mat4 ipvmat);

	//TODO: Convert this to output ivec2 to avoid problems with equalitites later on
	glm::vec2 snapToGrid(glm::vec2 location, float gridSX, float gridSY);
}