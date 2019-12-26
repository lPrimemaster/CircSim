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
	static ChunkCoord getChunkIndexAtPosition(glm::vec2 p);

	static bool checkChunkExists(glm::vec2 p);
	static bool checkChunkExists(ChunkCoord c);

	static bool isChunkPopulated(Chunk* chunk);
	static bool isChunkPopulated(glm::vec2 p);
	static bool isChunkPopulated(ChunkCoord c);

	static bool createChunkAtPosition(glm::vec2 p);
	static bool deleteChunkAtPosition(glm::vec2 p);

	static void allocateStart();

private:
	static HashTable<Chunk*, ChunkCoord> loaded;

private:
	ChunkManager() = delete;
};

