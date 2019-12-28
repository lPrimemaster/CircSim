#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "../Node.h"

struct Connector
{
	enum class IO
	{
		INPUT, OUTPUT
	}Type;
	glm::vec2 position;

	Connector()
	{
		Type = IO::INPUT;
		position = glm::vec2(0, 0);
		node = nullptr;
	}

	Connector(IO type, glm::vec2 pos)
	{
		Type = type;
		position = pos;
		node = nullptr;
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

	//TODO: Add the place the chunk is? Or are calculations not heavy ??
	std::vector<Connector*> dep_conector;

	Node* node;
};

template<size_t inputs, size_t outputs>
class InOutType
{
public:
	using InArray = std::array<Connector, inputs>;
	using OutArray = std::array<Connector, outputs>;

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

	//Connector getOutbound

protected:
	//Must be called on Derived Constructor
	inline virtual void defineConnectorDependencies() = 0; 

	//Must be called on Derived Constructor and on Update
	inline virtual void updateConnectors() = 0; 

protected:
	InArray in_ports;
	OutArray out_ports;
};

