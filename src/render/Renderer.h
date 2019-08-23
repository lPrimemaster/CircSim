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
	Renderer(const std::string& program, int flags);
	Renderer(const std::string& program);
	~Renderer();

	void push(Component* c);
	void pushList(Component** list, size_t size);

	void pop(Component* c);
	void popList(Component** list, size_t size);

	void setPVMatrix(const glm::mat4& pvm);

	virtual void render() = 0;

protected:
	std::vector<Component*> comps;
	glm::mat4 pvm;
	Program p;
};
