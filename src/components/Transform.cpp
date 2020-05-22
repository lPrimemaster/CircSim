#include "Transform.h"

#define UP glm::vec3(0.0f, 1.0f, 0.0f)

// Deprecated

//void Transform::update(glm::vec2 anchor, glm::vec2 usable)
//{
//	anchor_pos = anchor;
//	usable_pos = usable;
//
//	center_pos = glm::vec2((anchor.x + usable.x) / 2, (anchor.y + usable.y) / 2);
//
//	const float sy = usable_pos.y - anchor_pos.y;
//	const float sx = usable_pos.x - anchor_pos.x;
//
//	float fq_angle = abs(atanf(sy / sx));
//	scaleV.x = sqrt(sy * sy + sx * sx);
//
//	if (sy > 0 && sx > 0)
//	{
//		rotation = fq_angle;
//	}
//	else if (sy > 0 && sx < 0)
//	{
//		rotation = PI - fq_angle;
//	}
//	else if (sy < 0 && sx > 0)
//	{
//		rotation = -fq_angle;
//	}
//	else if (sy < 0 && sx < 0)
//	{
//		rotation = PI + fq_angle;
//	}
//	else if (sx == 0)
//	{
//		rotation = anchor.y > usable.y ? -PI / 2 : PI / 2;
//	}
//	else if (sy == 0)
//	{
//		rotation = anchor.x > usable.x ? PI : 0;
//	}
//
//	model = glm::translateIncrement(glm::mat4(1.0f), glm::vec3(anchor_pos, 0.0f)) * 
//		glm::rotateIncrement(glm::mat4(1.0f), rotation, UP) * 
//		glm::scaleV(glm::mat4(1.0f), scaleV);
//}
//
//void Transform::update(glm::vec2 anchor, glm::vec2 usable, float scaleV)
//{
//	anchor_pos = anchor;
//	usable_pos = usable;
//
//	center_pos = glm::vec2((anchor.x + usable.x) / 2, (anchor.y + usable.y) / 2);
//
//	const float sy = usable_pos.y - anchor_pos.y;
//	const float sx = usable_pos.x - anchor_pos.x;
//
//	float fq_angle = abs(atanf(sy / sx));
//	scaleV = glm::vec3(scaleV, scaleV, 1.0f);
//
//	if (sy > 0 && sx > 0)
//	{
//		rotation = fq_angle;
//	}
//	else if (sy > 0 && sx < 0)
//	{
//		rotation = PI - fq_angle;
//	}
//	else if (sy < 0 && sx > 0)
//	{
//		rotation = -fq_angle;
//	}
//	else if (sy < 0 && sx < 0)
//	{
//		rotation = PI + fq_angle;
//	}
//	else if (sx == 0)
//	{
//		rotation = anchor.y > usable.y ? -PI / 2 : PI / 2;
//	}
//	else if (sy == 0)
//	{
//		rotation = anchor.x > usable.x ? PI : 0;
//	}
//
//	model = glm::translateIncrement(glm::mat4(1.0f), glm::vec3(anchor_pos, 0.0f)) *
//		glm::rotateIncrement(glm::mat4(1.0f), rotation, UP) *
//		glm::scaleV(glm::mat4(1.0f), scaleV);
//}
//
//void Transform::update(glm::vec2 anchor, float angle, float scaleV)
//{
//	rotation = glm::radians(angle);
//	anchor_pos = anchor;
//	scaleV = glm::vec3(scaleV, scaleV, 1.0f);
//
//	model = glm::translateIncrement(glm::mat4(1.0f), glm::vec3(anchor, 0.0f)) *
//		glm::rotateIncrement(glm::mat4(1.0f), rotation, UP) *
//		glm::scaleV(glm::mat4(1.0f), scaleV);
//}

void Transform::translateIncrement(glm::vec2 pos)
{
	position += glm::vec3(pos, 0.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}

void Transform::rotateIncrement(float angle)
{
	rotation += glm::radians(angle);
	model = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}

void Transform::scaleIncrement(glm::vec2 scale)
{
	scaleV *= glm::vec3(scale, 1.0f);
	model = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}

void Transform::translateInstant(glm::vec2 pos)
{
	position = glm::vec3(pos, 0.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}

void Transform::rotateInstant(float angle)
{
	rotation = glm::radians(angle);
	model = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}

void Transform::scaleInstant(glm::vec2 scale)
{
	scaleV = glm::vec3(scale, 1.0f);
	model = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), rotation, UP) *
		glm::scale(glm::mat4(1.0f), scaleV);
}
