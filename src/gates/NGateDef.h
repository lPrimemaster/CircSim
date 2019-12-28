#pragma once
#include <iostream>
#include <vector>
#include "../geometry/Component.h"
#include "InOutType.h"
#include "Gate.h"

class NotGate : public Gate, public InOutType<1, 1>
{
public:
	NotGate();
	~NotGate();

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

protected:
	inline void defineConnectorDependencies() override
	{
		out_ports[0].dep_conector.push_back(&in_ports[0]);
	}

	inline void updateConnectors() override
	{
		out_ports[0].Type = Connector::IO::OUTPUT;
		out_ports[0].position = out;

		in_ports[0].Type = Connector::IO::INPUT;
		in_ports[0].position = in;
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

