#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Connector
{
	enum class IO
	{
		INPUT, OUTPUT
	}Type;
	glm::vec2 position;

	Connector(IO type, glm::vec2 pos)
	{
		Type = type;
		position = pos;
	}

	bool operator==(const Connector& rhs) const
	{
		return (Type == rhs.Type);
	}

	bool operator!=(const Connector& rhs) const
	{
		return (Type != rhs.Type);
	}

	bool isConnected(const Connector& to) const
	{
		return (*this != to && position == to.position);
	}
};

template<size_t inputs, size_t outputs>
class InOutType
{
public:
	using InArray = std::array<Connector, inputs>;
	using OutArray = std::array<Connector, outputs>;

	virtual ~InOutType() = 0;

	inline InArray getInputs() const
	{
		return in_ports;
	}

	inline OutArray getOutputs() const
	{
		return out_ports;
	}

	Connector getOutbound

protected:
	InArray in_ports;
	OutArray out_ports;
};

