#pragma once
#include <cstdlib>
#include <memory>
#include <deque>

#include "Chunk.h"

//TODO: A chunk is only created if there is any gate inside it (destroyed if gate is removed)
class ChunkManager
{
public:
	static Chunk* getChunkAtPosition(glm::vec2 p);
	static Chunk* getChunkAtPosition(ChunkCoord c);

	static ChunkCoord getChunkIndexAtPosition(glm::vec2 p);

	static bool checkChunkExists(glm::vec2 p);
	static bool checkChunkExists(ChunkCoord c);

	static bool isChunkPopulated(Chunk* chunk);
	static bool isChunkPopulated(glm::vec2 p);
	static bool isChunkPopulated(ChunkCoord c);

	static bool createChunkAtPosition(ChunkCoord c);
	static bool createChunkAtPosition(glm::vec2 p);

	static bool deleteChunkAtPosition(ChunkCoord c);
	static bool deleteChunkAtPosition(glm::vec2 p);

	static void allocateStart();

	static void populateChunks(std::vector<Gate*>* global_list);

	static std::vector<Node*> updateConnectorNode(Connector* c);
	static std::vector<Node*> updateInteractConnectorNode(InteractConnector* c);

private:
	static void workerPopulate(std::vector<Gate*>::iterator start, std::vector<Gate*>::iterator end);

private:
	static HashTable<Chunk*, ChunkCoord> loaded;

private:
	ChunkManager() = delete;
};

