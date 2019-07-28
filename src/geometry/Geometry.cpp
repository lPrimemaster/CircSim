#include "Geometry.h"

std::unordered_map<std::string, Geometry*> Geometry::rGeometries;

Geometry::Geometry(DrawType type, std::vector<glm::vec2> points)
{
	this->size = points.size();
	this->type = type;
	this->gpuByteSize = points.size() * sizeof(glm::vec2);

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->gpuByteSize, &points.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

Geometry::Geometry(DrawType type, float* points, const size_t size)
{
	this->size = size / 2;
	this->type = type;
	this->gpuByteSize = size * sizeof(float);

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->gpuByteSize, points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Geometry::unregisterGeometry(const std::string& name)
{
	delete rGeometries[name];
}

void Geometry::getStatistics()
{
	size_t bCount = rGeometries.bucket_count();
	size_t tbSize = 0;
	for (auto g : rGeometries)
	{
		tbSize += g.second->gpuByteSize;
	}

	std::cerr << "Registered Geometry gpu (bytes) = " << tbSize << std::endl;
	std::cerr << "Geometry hash table bucket count = " << bCount << std::endl;
}

void Geometry::rehash(size_t value)
{
	rGeometries.rehash(value);
}

void Geometry::registerGeometry(Geometry* g, const std::string& name)
{
	rGeometries.emplace(name, g);
}
