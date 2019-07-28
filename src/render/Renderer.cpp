#include "Renderer.h"


Renderer::Renderer() : p("color")
{
}

Renderer::~Renderer()
{
	comps.clear();
}

void Renderer::push(Component* c)
{
	comps.push_back(c);
}

void Renderer::pop(Component* c)
{
	std::vector<Component*>::iterator found = std::find(comps.begin(), comps.end(), c);
	if (found != comps.end())
	{
		comps.erase(found);
	}
}

void Renderer::pushList(Component** list, size_t size)
{
	//Push according to list's order
	for (int i = 0; i < size; i++)
	{
		push(list[i]);
	}
}

void Renderer::render()
{
	p.bind();
	p.loadMatrix4f("PView", pvm);
	for (auto c : comps)
	{
		Geometry* geo = Geometry::getRegisteredGeometry(c->geometryName());
		p.loadVector4f("color", c->getColor());
		p.loadMatrix4f("Model", c->transform().getTransform());

		geo->bind();
		glDrawArrays(geo->getType(), 0, geo->getSize());
	}
}

void Renderer::setProjectViewMatrix(const glm::mat4& pvm)
{
	this->pvm = pvm;
}
