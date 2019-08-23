#pragma once
#include "math.h"

glm::vec2 unprojectPoint(glm::vec3 ndc, glm::mat4 transform)
{
	glm::vec4 projPos = glm::inverse(transform) * glm::vec4(ndc, 1.0f);
	return glm::vec2(projPos.x, projPos.y) / projPos.w;
}

glm::vec2 math::screenToWorld(glm::vec2 coord, glm::mat4 ipvmat)
{
	//Get framebuffer size
	ImGuiIO& io = ImGui::GetIO();

	float w = io.DisplaySize.x;
	float h = io.DisplaySize.y;
	float r = w / h;

	//Coord to NDC (y inverted)
	float ndc_x = (((coord.x / w) * 2.0f) - 1.0f);
	float ndc_y = (((coord.y / h) * 2.0f) - 1.0f);

	glm::vec3 mworld = ipvmat * glm::vec4(ndc_x, -ndc_y, 0.0f, 1.0f);

	return glm::vec2(mworld);
}