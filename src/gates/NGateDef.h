#pragma once
#include <iostream>
#include <vector>
#include "../geometry/Component.h"
#include "InOutType.h"
#include "Gate.h"

#define GCC(i) components[i]->getColor()
#define GET_COLOR_INIT_LIST() { GCC(0), GCC(1), GCC(2), GCC(3), GCC(4), GCC(5), GCC(6), GCC(7) }

class NotGate : public Gate, public InOutType<1, 1>
{
public:
	NotGate();
	~NotGate();

	void update(const glm::vec2 in, const glm::vec2 out) override;
	void updateInput(const unsigned state) override;

	inline glm::vec4 getAB() override
	{
		glm::vec4 ret;
		ret.x = in.x;
		ret.y = in.y;
		ret.z = out.x;
		ret.w = out.y;
		return ret;
	}

	inline GraphicComponent* getComponent(int index) override
	{
		return components[index];
	}

	inline GraphicComponent** getComponentList() override
	{
		return components;
	}

	const inline size_t getComponentListSize() override
	{
		return 8;
	}

	inline void changeAlpha(float alpha) override
	{
		for (int i = 0; i < 8; i++)
		{
			glm::vec4 oc = components[i]->getColor();
			components[i]->setColor(glm::vec4(oc.r, oc.g, oc.b, alpha));
		}
	}

	inline void changeColor(glm::vec3 color = glm::vec3(0.0f))
	{
		static glm::vec4 original_color[8] = GET_COLOR_INIT_LIST();

		if (color == glm::vec3(0.0f))
		{
			for (int i = 0; i < 8; i++)
			{
				components[i]->setColor(original_color[i]);
			}
			return;
		}

		for (int i = 0; i < 8; i++)
		{
			glm::vec4 oc = components[i]->getColor();
			components[i]->setColor(glm::vec4(color, oc.a));
		}
	}

protected:
	inline void defineConnectorDependencies() override
	{
		out_ports[0] = new Connector();
		in_ports[0] = new Connector();
		out_ports[0]->dep_conector.push_back(in_ports[0]);
	}

	inline void updateConnectors() override
	{
		out_ports[0]->Type = Connector::IO::OUTPUT;
		out_ports[0]->position = out;

		in_ports[0]->Type = Connector::IO::INPUT;
		in_ports[0]->position = in;
	}

private:
	GraphicComponent* components[8];
	glm::vec2 in = glm::vec2(0.0f, 1.0f);
	glm::vec2 out = glm::vec2(1.0f, 0.0f);

	const float Tscale = 0.4f;
	const float Cscale = 0.04f;

private:
	glm::vec2 getTriangleCenter();
};

#undef GCC
#undef GET_COLOR_INIT_LIST
