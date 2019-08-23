#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "../render/gui/Gui.h"

#define PI_F 3.141592654f

namespace math
{
	glm::vec2 screenToWorld(glm::vec2 coord, glm::mat4 ipvmat);
}