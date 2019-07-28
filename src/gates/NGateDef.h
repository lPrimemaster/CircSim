#pragma once
#include <iostream>
#include "../geometry/Component.h"

class NGateDef
{
public:
	NGateDef();
	~NGateDef();

	void update(const glm::vec2 in, const glm::vec2 out);
	void updateInput(const unsigned state);

	inline Component* getComponent(int index)
	{
		return components[index];
	}

	inline Component** getComponentList()
	{
		return components;
	}

	static const inline size_t GetComponentListSize()
	{
		return 8;
	}

private:
	Component* components[8];
	glm::vec2 in = glm::vec2(0.0f, 1.0f);
	glm::vec2 out = glm::vec2(1.0f, 0.0f);

	const float Tscale = 0.4f;
	const float Cscale = 0.04f;

private:
	glm::vec2 getTriangleCenter();
};

