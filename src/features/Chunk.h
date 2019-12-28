#pragma once
#include <iostream>
#include "../gates/NGateDef.h"
#include "../util/HashTable.h"
#include "../CSim.h"
#include <vector>
#include <future>

#define CHUNK_SIZE 6.0f

struct ChunkCoord : public Hashable
{
	int chunk_id_x = 0;
	int chunk_id_y = 0;

	size_t operator()(const Hashable& h) const override
	{
		return ((chunk_id_y * 0x1f1f1f1f) ^ chunk_id_x);
	}

	bool operator==(const Hashable& rhs) const override
	{
		ChunkCoord recast = (static_cast<const ChunkCoord&>(rhs));
		return chunk_id_x == recast.chunk_id_x && chunk_id_y == recast.chunk_id_y;
	}

};

//TODO: Each chunk should manage its own gates - and not to be done inside the main state
class Chunk
{
public:
	friend class ChunkManager;

	Chunk(ChunkCoord coord);
	Chunk(int x, int y);
	~Chunk() = default;

	bool isInside(glm::vec2 value);

	void insertConnector(Connector* c);
	void deleteConnector(Connector* c);

	bool isEmpty() const;

private:
	void updateNodeInternal(Connector* c, Connector* d);

private:
	std::vector<Connector*> connector_list;
	std::vector<Connector*> connector_update;

	ChunkCoord coord;
};

