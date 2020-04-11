#include "Renderer.h"


Renderer::Renderer(const std::string& program, int flags) : p(program, flags) {  }
Renderer::Renderer(const std::string& program) : p(program, 0) {  }

Renderer::~Renderer()
{
	comps.clear();
}

void Renderer::push(GraphicComponent* c)
{
	comps.push_back(c);
}

void Renderer::pop(GraphicComponent* c)
{
	std::vector<GraphicComponent*>::iterator found = std::find(comps.begin(), comps.end(), c);
	if (found != comps.end())
	{
		comps.erase(found);
	}
}

void Renderer::popList(GraphicComponent** list, size_t size)
{
	//Pop according to list's order
	for (int i = 0; i < size; i++)
	{
		pop(list[i]);
	}
}

void Renderer::pushList(GraphicComponent** list, size_t size)
{
	//Push according to list's order
	for (int i = 0; i < size; i++)
	{
		push(list[i]);
	}
}

void Renderer::setPVMatrix(const glm::mat4& pvm)
{
	this->pvm = pvm;
}
