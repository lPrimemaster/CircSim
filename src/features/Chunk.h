#pragma once
#include <iostream>
#include "../gates/NGateDef.h"
#include "../util/HashTable.h"
#include <vector>
#include <future>

#define CHUNK_SIZE 6.0f

struct ChunkCoord : public Hashable
{
	int chunk_id_x;
	int chunk_id_y;

	size_t operator()(const Hashable& h) const override
	{
		//TODO: PLEASE CHANGE THIS LATER FOR A BETTER HASH FUNC
		return (std::hash<int>()(chunk_id_x) ^ std::hash<int>()(chunk_id_y));
	}

	bool operator==(const Hashable& rhs) const override
	{
		ChunkCoord recast = (static_cast<const ChunkCoord&>(rhs));
		return chunk_id_x == recast.chunk_id_x && chunk_id_y == recast.chunk_id_y;
	}

};

struct GateIOResult
{
	bool has_from = false;
	bool has_to = false;

	glm::vec2 from;
	glm::vec2 to;
};

//TODO: Each chunk should manage its own gates - and not to be done inside the main state
class Chunk
{
public:
	Chunk(ChunkCoord coord);
	Chunk(int x, int y);
	~Chunk() = default;

	bool isInside(glm::vec2 value);
	GateIOResult getConnections(Gate* gate);
	void insertGate(Gate* gate);
	void deleteGate(Gate* gate);

	bool isEmpty() const;

private:
	std::vector<Gate*> gate_list;

	ChunkCoord coord;
};

