#pragma once
#include "../core/ECS.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159265358979f

class Transform : public FCS::Component
{
public:
	void update(const Transform transform);
	void update(glm::vec2 anchor, glm::vec2 usable);
	void update(glm::vec2 anchor, glm::vec2 usable, float scale);
	void update(glm::vec2 anchor, float angle = 0.0f, float scale = 1.0f);

	void translate(glm::vec2 pos);
	void rotate(float angle);
	void scale(glm::vec2 scale);

	inline glm::mat4 getModelMatrix() const
	{
		return model;
	}

	inline glm::vec3 getPosition() const
	{
		return position;
	}

	inline glm::vec2 getScale() const
	{
		return glm::vec2(scaleAxis.x, scaleAxis.y);
	}

	bool& isOnScreen()
	{
		return on_screen;
	}

private:
	glm::vec2 anchor_pos = glm::vec2();
	glm::vec2 usable_pos = glm::vec2();
	glm::vec2 center_pos = glm::vec2();

	glm::vec3 position;

	float rotation = 0.0f;
	glm::vec3 scaleAxis = { 1.0f, 1.0f, 1.0f };

	glm::mat4 model = glm::mat4(1.0f);

	bool on_screen = false;

	FCS_COMPONENT(Transform);
};

