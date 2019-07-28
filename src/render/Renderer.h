#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "../geometry/Transform.h"
#include "../geometry/Geometry.h"
#include "../geometry/Component.h"
#include "Program.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void push(Component* c);
	void pop(Component* c);

	void pushList(Component** list, size_t size);

	void render();
	void setProjectViewMatrix(const glm::mat4& pvm);

private:
	std::vector<Component*> comps;
	glm::mat4 pvm;

private:
	Program p;
};
