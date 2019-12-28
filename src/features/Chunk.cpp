#include "Chunk.h"

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

void Chunk::insertConnector(Connector* c)
{
	connector_list.push_back(c);
}

void Chunk::deleteConnector(Connector* c)
{
	auto found = std::find(connector_list.cbegin(), connector_list.cend(), c);
	if (found != connector_list.cend())
	{
		connector_list.erase(found);
	}
}

bool Chunk::isEmpty() const
{
	return connector_list.empty();
}
