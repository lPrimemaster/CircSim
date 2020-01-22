#include "ChunkManager.h"
#include "../util/math.h"

HashTable<Chunk*, ChunkCoord> ChunkManager::loaded;

Chunk* ChunkManager::getChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	return getChunkAtPosition(cc);
}

Chunk* ChunkManager::getChunkAtPosition(ChunkCoord c)
{
	Chunk* chunk = nullptr;
	if (checkChunkExists(c))
	{
		try
		{
			chunk = loaded.at(c);
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

bool ChunkManager::createChunkAtPosition(ChunkCoord c)
{
	if (checkChunkExists(c))
	{
		//Chunk exists - don't mind this function call
		return false;
	}

	loaded.try_emplace(c, new Chunk(c));

	return true;
}

bool ChunkManager::createChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	return createChunkAtPosition(cc);
}

bool ChunkManager::deleteChunkAtPosition(ChunkCoord c)
{
	if (!checkChunkExists(c))
	{
		//Chunk doesn't exist - don't mind this function call
		return false;
	}

	loaded.erase(c);

	return true;
}

bool ChunkManager::deleteChunkAtPosition(glm::vec2 p)
{
	ChunkCoord cc = getChunkIndexAtPosition(p);

	return deleteChunkAtPosition(cc);
}

std::vector<Chunk*> ChunkManager::getAllLoadedChunks()
{
	auto map = loaded.getRawConst();
	std::vector<Chunk*> ret;
	std::transform(map.begin(), map.end(), std::back_inserter(ret), math::gMapSecond(map));
	return ret;
}

void ChunkManager::allocateStart()
{
	//Home chunk
	ChunkCoord cc;
	cc.chunk_id_x = 0;
	cc.chunk_id_y = 0;

	loaded.try_emplace(cc, new Chunk(cc));
}

//Deprecated
void ChunkManager::populateChunks(std::vector<Gate*>* global_list)
{
	//Iterate trough all gates and put them into their specific chunk - generated at run time
	//This should take long
	//Speed up using async processing
	using GateIt = std::vector<Gate*>::iterator;
	static const unsigned int HC = std::thread::hardware_concurrency();

	size_t size = global_list->size();
	size_t parcial_val = size / HC;

	GateIt start = global_list->begin();

	std::vector<std::future<void>> fs;

	for(unsigned int i = 0; i < HC - 1; i++)
		fs.push_back(std::async(workerPopulate, start + parcial_val * i, start + parcial_val * (i + 1)));

	size_t left_over = size - (parcial_val * HC);
	fs.push_back(std::async(workerPopulate, start + parcial_val * HC, start + parcial_val * HC + left_over));

	for (auto& p : fs)
	{
		p.wait();
	}
}

void ChunkManager::workerPopulate(std::vector<Gate*>::iterator start, std::vector<Gate*>::iterator end)
{
	using GateIt = std::vector<Gate*>::iterator;
	for (GateIt it = start; it != end + 1; it++)
	{
		//Find out where the gate has its connectors

	}
}
