#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "../util/math.h"
#include "../Node.h"

struct Connector
{
	enum class IO
	{
		INPUT, OUTPUT
	}Type;
	glm::vec2 position;

	typedef int Flag;

	Connector(Flag useMark = 1)
	{
		Type = IO::INPUT;
		position = glm::vec2(0, 0);
		node = nullptr;
		if (useMark)
			mark = new Sprite("Circle");
		else
			mark = nullptr;
	}

	Connector(IO type, glm::vec2 pos, Flag useMark = 1)
	{
		Type = type;
		position = pos;
		node = nullptr;
		if (useMark)
			mark = new Sprite("Circle");
		else
			mark = nullptr;
	}

	~Connector()
	{
		if (mark)
			delete mark;
	}

	virtual const bool isInteractible() const
	{
		return interactible;
	}

	virtual void updateState(bool input)
	{
		//Do nothing
	}

	bool operator==(const Connector& rhs) const
	{
		return (Type == rhs.Type);
	}

	bool operator!=(const Connector& rhs) const
	{
		return (Type != rhs.Type);
	}

	bool isMerged(const Connector& with) const
	{
		return (*this != with && position == with.position);
	}

	bool isOverlapped(const Connector& with) const
	{
		return (position == with.position);
	}

	std::vector<Connector*> dep_conector;

	Node* node;
	Sprite* mark;

	bool interactible = false;
};

struct InteractConnector : public Connector
{
	InteractConnector() : Connector(0)
	{
		interactible = true;
	}

	InteractConnector(IO type, glm::vec2 pos) : Connector(type, pos, 0)
	{
		interactible = true;
	}

	virtual const bool isInteractible() const override
	{
		return true;
	}

	virtual inline bool checkMouse(glm::vec2 mouse_world) = 0;

	virtual inline void onClick() = 0;

	virtual void updateState(bool input)
	{
		node->write(input);
	}

	Connector* output_interact = nullptr;

	float click_area_of_effect = 0.04f;

	bool state = false;
};

struct SwitchConnector : public InteractConnector
{
	SwitchConnector(float aoe = 0.04f) : InteractConnector()
	{
		click_area_of_effect = aoe;
	}

	SwitchConnector(IO type, glm::vec2 pos, float aoe = 0.04f) : InteractConnector(type, pos)
	{
		click_area_of_effect = aoe;
	}

	inline bool checkMouse(glm::vec2 mouse_world) override
	{
		return math::isInsideRadius(mouse_world, position, click_area_of_effect);
	}

	inline void onClick() override
	{
		updateState(!state);
		state = !state;
	}

	float click_area_of_effect = 0.04f;
};

template<size_t inputs, size_t outputs>
class InOutType
{
public:
	using InArray = std::array<Connector*, inputs>;
	using OutArray = std::array<Connector*, outputs>;

	InOutType() = default;
	virtual ~InOutType() {  };


	inline InArray& getInputs()
	{
		return in_ports;
	}

	inline OutArray& getOutputs()
	{
		return out_ports;
	}

protected:
	//Must be called on Derived Constructor
	inline virtual void defineConnectorDependencies() = 0; 

	//Must be called on Derived Constructor and on Update
	inline virtual void updateConnectors() = 0; 

protected:
	InArray in_ports;
	OutArray out_ports;
};

