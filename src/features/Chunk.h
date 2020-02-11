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

	ChunkCoord() = default;
	ChunkCoord(std::initializer_list<int> l) { chunk_id_x = *l.begin(); chunk_id_y = *(l.begin() + 1); }

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

class Chunk
{
public:
	friend class ChunkManager;
	friend class ConnectorManager;

	Chunk(ChunkCoord coord);
	Chunk(int x, int y);
	~Chunk() = default;

	bool isInside(glm::vec2 value);

	inline std::vector<InteractConnector*> getInteractorsList() const
	{
		return interactors_list;
	}

	inline std::vector<Connector*> getConnectorsList() const
	{
		return connector_list;
	}

	inline std::vector<Gate*> getGateList() const
	{
		return gate_list;
	}

	void insertConnector(Connector* c);
	void deleteConnector(Connector* c);

	void insertGate(Gate* g);
	void deleteGate(Gate* g);

	bool isEmpty() const;

private:
	void updateNodeInternal(Connector* c, Connector* d);

private:
	std::vector<Connector*> connector_list;
	std::vector<InteractConnector*> interactors_list;
	std::vector<Gate*> gate_list;

	ChunkCoord coord;
};

