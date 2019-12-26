#include "Chunk.h"

static std::tuple<glm::vec2, glm::vec2, bool, bool> findPins(GateIOPos& other, GateIOPos& self)
{
	glm::vec2 in;
	glm::vec2 out;
	bool found_in = false;
	bool found_out = false;

	if (other.out[0] == self.out[0])
	{
		out = other.out[0]; found_out = true;
	}
	else if(other.out[0] == self.in[0])
	{
		in = other.out[0]; found_in = true;
	}

	if (other.in[0] == self.out[0])
	{
		out = other.in[0]; found_out = true;
	}
	else if (other.in[0] == self.in[0])
	{
		in = other.in[0]; found_in = true;
	}

	return std::make_tuple<glm::vec2&, glm::vec2&, bool&, bool&>(in, out, found_in, found_out);
}

Chunk::Chunk(ChunkCoord coord) : coord(coord)
{

}

Chunk::Chunk(int x, int y) : coord()
{
	coord.chunk_id_x = x;
	coord.chunk_id_y = y;
}

bool Chunk::isInside(glm::vec2 value)
{
	float lowX = coord.chunk_id_x * CHUNK_SIZE - CHUNK_SIZE / 2.0f;
	float lowY = coord.chunk_id_y * CHUNK_SIZE - CHUNK_SIZE / 2.0f;

	float highX = lowX + CHUNK_SIZE;
	float highY = lowY + CHUNK_SIZE;

	//Don't account for borders
	return (lowX < value.x < highX && lowY < value.y < highY);
}

//TODO: Should we rather check against nodes?
GateIOResult Chunk::getConnections(Gate* gate)
{
	GateIOResult result;

	GateIOPos io = gate->getIOPositions();

	for (auto g : gate_list)
	{
		GateIOPos gio = g->getIOPositions();
		auto found = findPins(gio, io);

		//Found in
		if (!result.has_from && std::get<2>(found))
		{
			result.from = std::get<0>(found);
			result.has_from = true;
		}

		//Found out
		if (!result.has_to && std::get<3>(found))
		{
			result.to = std::get<1>(found);
			result.has_to = true;
		}

		if (result.has_from && result.has_to)
		{
			return result;
		}
	}

	return result;
}

void Chunk::insertGate(Gate* gate)
{
	gate_list.push_back(gate);
}

void Chunk::deleteGate(Gate* gate)
{
	auto found = std::find(gate_list.cbegin(), gate_list.cend(), gate);
	if (found != gate_list.cend())
	{
		gate_list.erase(found);
	}
}

bool Chunk::isEmpty() const
{
	return gate_list.empty();
}
