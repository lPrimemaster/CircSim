#pragma once
#include <iostream>
#include <vector>
#include "../geometry/Component.h"
#include "InOutType.h"
#include "Gate.h"
#include "../util/math.h"

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

protected:
	inline void defineConnectorDependencies() override
	{
		out_ports[0] = new Connector();
		in_ports[0] = new SwitchConnector(Cscale);

		auto loc = &(out_ports[0]->node);
		loc = &(in_ports[0]->node);
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

