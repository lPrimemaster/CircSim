#pragma once
#include <iostream>
#include <vector>
#include "../geometry/Component.h"
#include "InOutType.h"
#include "Gate.h"
//#include "../util/math.h"

#define GCC(i) components[i]->getColor()
#define GET_COLOR_INIT_LIST() { GCC(0), GCC(1), GCC(2), GCC(3), GCC(4), GCC(5), GCC(6), GCC(7) }

class SwitchGate : public Gate, public InOutType<1, 1>
{
public:
	SwitchGate();
	~SwitchGate();

	void update(const glm::vec2 center, const glm::vec2 out) override;
	void updateInput(const unsigned state) override;

	inline Component* getComponent(int index) override
	{
		return components[index];
	}

	inline Component** getComponentList() override
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
		in_ports[0] = new SwitchConnector(Cscale);

		dynamic_cast<SwitchConnector*>(in_ports[0])->output_interact = out_ports[0];
	}

	inline void updateConnectors() override
	{
		in_ports[0]->Type = Connector::IO::INPUT;
		in_ports[0]->position = center;

		out_ports[0]->Type = Connector::IO::OUTPUT;
		out_ports[0]->position = out;
	}

private:
	Component* components[8];
	glm::vec2 out = glm::vec2(2.0f, 0.0f);
	glm::vec2 center = glm::vec2(0.0f, 0.0f);

	const float Tscale = 0.2f;
	const float Cscale = 0.04f;
};

#undef GCC(i)
#undef GET_COLOR_INIT_LIST()
