#pragma once
#include "math.h"

glm::vec2 unprojectPoint(glm::vec3 ndc, glm::mat4 transform)
{
	glm::vec4 projPos = glm::inverse(transform) * glm::vec4(ndc, 1.0f);
	return glm::vec2(projPos.x, projPos.y) / projPos.w;
}

glm::vec2 math::screenToWorld(glm::vec2 wdim, glm::vec2 coord, glm::mat4 ipvmat)
{
	//Get framebuffer size
	float w = wdim.x;
	float h = wdim.y;
	float r = w / h;

	//Coord to NDC (y inverted)
	float ndc_x = (((coord.x / w) * 2.0f) - 1.0f);
	float ndc_y = (((coord.y / h) * 2.0f) - 1.0f);

	glm::vec3 mworld = ipvmat * glm::vec4(ndc_x, -ndc_y, 0.0f, 1.0f);

	return glm::vec2(mworld);
}

glm::vec2 math::snapToGrid(glm::vec2 location, float gridSX, float gridSY)
{
	float x_snap = roundf(location.x / gridSX) * gridSX;
	float y_snap = roundf(location.y / gridSY) * gridSY;

	return glm::vec2(x_snap, y_snap);
}

glm::vec2 math::polarToCartesian(float angle, float distance)
{
	return distance * glm::vec2(std::cos(angle), std::sin(angle));
}

math::pvec2 math::cartesianToPolar(glm::vec2 vector)
{
	pvec2 ret;
	ret.distance = glm::length(vector);
	ret.angle = std::atan(vector.y / vector.x);

	if (vector.y < 0.0f)
	{
		ret.angle += PI_F;
	}

	return ret;
}

bool math::isInsideRadius(glm::vec2 point, glm::vec2 center, float radius)
{
	return (std::powf(point.x - center.x, 2) + std::powf(point.y - center.y, 2) < std::powf(radius, 2));
}
