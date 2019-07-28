#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159265358979f

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

	void update(const Transform transform);
	void update(glm::vec2 anchor, glm::vec2 usable);
	void update(glm::vec2 anchor, glm::vec2 usable, float scale);
	void update(glm::vec2 anchor, float angle = 0.0f, float scale = 1.0f);

	void rotate(float angle);
	void scale(glm::vec2 scale);

	inline glm::mat4 getTransform() const
	{
		return model;
	}

private:
	glm::vec2 anchor_pos;
	glm::vec2 usable_pos;
	glm::vec2 center_pos;

	float rotation = 0.0f;
	glm::vec3 scaleAxis = { 1.0f, 1.0f, 1.0f };

	glm::mat4 model;
};

