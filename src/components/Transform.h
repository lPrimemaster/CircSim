#pragma once
#include "../core/ECS.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159265358979f

class Transform : public FCS::Component
{
public:
	void translateIncrement(glm::vec2 pos);
	void rotateIncrement(float angle);
	void scaleIncrement(glm::vec2 scale);

	void translateInstant(glm::vec2 pos);
	void rotateInstant(float angle);
	void scaleInstant(glm::vec2 scale);

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
		return glm::vec2(scaleV.x, scaleV.y);
	}

	inline float getRotation() const
	{
		return rotation;
	}

	bool& isOnScreen()
	{
		return on_screen;
	}

private:
	glm::vec3 position = glm::vec3();
	glm::vec3 scaleV = glm::vec3(1.0f);
	float rotation = 0.0f;

	glm::mat4 model = glm::mat4(1.0f);

	bool on_screen = false;

	FCS_COMPONENT(Transform);
};

