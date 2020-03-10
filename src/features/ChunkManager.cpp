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

std::vector<ChunkCoord> ChunkManager::getValidNeighbourChunks(ChunkCoord c, math::BRect obb)
{
	std::vector<ChunkCoord> neighbours;
	
	//Check for adjacent chunks
	auto checkBoundsInside = [obb, &neighbours](ChunkCoord coord)
	{
		float lowX = (2.0f * coord.chunk_id_x - 1) * CHUNK_SIZE;
		float lowY = (2.0f * coord.chunk_id_y - 1) * CHUNK_SIZE;

		float highX = lowX + CHUNK_SIZE * 2.0f;
		float highY = lowY + CHUNK_SIZE * 2.0f;

		math::BRect chunk_bound = math::BRect(0, glm::vec2(lowX, highY), CHUNK_SIZE * 2.0f, CHUNK_SIZE * 2.0f);

		if (chunk_bound.intersect(obb))
			neighbours.push_back(coord);
	};

	checkBoundsInside({ c.chunk_id_x , c.chunk_id_y + 1 }); //Top
	checkBoundsInside({ c.chunk_id_x , c.chunk_id_y - 1 }); //Bottom
	checkBoundsInside({ c.chunk_id_x - 1 , c.chunk_id_y }); //Left
	checkBoundsInside({ c.chunk_id_x + 1 , c.chunk_id_y }); //Right

	return neighbours;
}

struct OBB_Chunk_IntersectResult
{
	std::vector<ChunkCoord> visited;
};

std::vector<Chunk*> ChunkManager::getAllOBBIntersect(ChunkCoord first, math::BRect obb)
{
	OBB_Chunk_IntersectResult ocir;

	ocir.visited.push_back(first);
	auto vnc = getValidNeighbourChunks(first, obb);

	while (!vnc.empty())
	{
		std::vector<ChunkCoord> local_coords;
		for (auto it = vnc.begin(); it != vnc.end(); it++)
		{
			auto other = getValidNeighbourChunks(*it, obb);
			local_coords.insert(local_coords.end(), other.begin(), other.end());
		}

		//Erase the neighbours that were already processed before
		local_coords.erase(std::remove_if(local_coords.begin(), local_coords.end(), 
			[&](ChunkCoord c) 
			{ 
				for (auto v : ocir.visited)
				{
					if (c == v)
						return true;
				}
				return false;
			}), local_coords.end());

		//Add new valid chunks to visited list
		ocir.visited.insert(ocir.visited.end(), vnc.begin(), vnc.end());

		//Remove possible duplicates from adjacent chunks
		math::removeDuplicates(local_coords);

		vnc.clear();
		vnc = local_coords;
	}

	std::vector<Chunk*> vchunks;

	for (auto v : ocir.visited)
	{
		if (!checkChunkExists(v))
		{
			createChunkAtPosition(v);
		}
		vchunks.push_back(getChunkAtPosition(v));
	}

	return vchunks;
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
