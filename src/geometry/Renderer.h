#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>

#include "Transform.h"
#include "Geometry.h"
#include "Component.h"
#include "Program.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	template<typename G>
	void push(Component* c);
	void pop(Component* c);

	inline void render();

private:
	std::unordered_map<Component*, const Geometry*> comps;

private:
	Program p;
};

template<typename G>
inline void Renderer::push(Component* c)
{
	const std::type_info& ti = typeid(G);
	const Geometry* g = Geometry::getRegisteredGeometry(ti);
	comps.emplace(c, g);
}

inline void Renderer::render()
{
	for (auto c : comps)
	{
		c.second->bind();
		glm::mat4 model = c.first->getTransform().getTransform();

	}
}
