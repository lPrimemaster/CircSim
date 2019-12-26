#include "ChunkManager.h"

HashTable<Chunk*, ChunkCoord> ChunkManager::loaded;

Chunk* ChunkManager::getChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	Chunk* chunk = nullptr;
	if (checkChunkExists(cc))
	{
		try
		{
			chunk = loaded.at(cc);
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return chunk;
}

ChunkCoord ChunkManager::getChunkIndexAtPosition(glm::vec2 p)
{
	ChunkCoord cc;
	cc.chunk_id_x = (int)floorf((p.x + CHUNK_SIZE) / (2.0f * CHUNK_SIZE));
	cc.chunk_id_y = (int)floorf((p.y + CHUNK_SIZE) / (2.0f * CHUNK_SIZE));

	return cc;
}

bool ChunkManager::checkChunkExists(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);
	return checkChunkExists(cc);
}

bool ChunkManager::checkChunkExists(ChunkCoord c)
{
	return loaded.evaluate(c);
}

bool ChunkManager::isChunkPopulated(Chunk* chunk)
{
	return chunk->isEmpty();
}

bool ChunkManager::isChunkPopulated(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);
	return isChunkPopulated(cc);
}

bool ChunkManager::isChunkPopulated(ChunkCoord c)
{
	Chunk* chunk = loaded.at(c);
	return isChunkPopulated(chunk);
}

bool ChunkManager::createChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	if (checkChunkExists(cc))
	{
		//Chunk exists - don't mind this function call
		return false;
	}

	loaded.try_emplace(cc, new Chunk(cc));

	return true;
}

bool ChunkManager::deleteChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	if (!checkChunkExists(cc))
	{
		//Chunk doesn't exist - don't mind this function call
		return false;
	}

	loaded.erase(cc);

	return true;
}

void ChunkManager::allocateStart()
{
	//Home chunk
	ChunkCoord cc;
	cc.chunk_id_x = 0;
	cc.chunk_id_y = 0;

	loaded.try_emplace(cc, new Chunk(cc));
}
