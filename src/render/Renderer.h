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

	virtual void push(GraphicComponent* c);
	virtual void pushList(GraphicComponent** list, size_t size);

	virtual void pop(GraphicComponent* c);
	virtual void popList(GraphicComponent** list, size_t size);

	virtual void setPVMatrix(const glm::mat4& pvm);

	virtual void render() = 0;

protected:
	std::vector<GraphicComponent*> comps;
	glm::mat4 pvm;
	Program p;
};
