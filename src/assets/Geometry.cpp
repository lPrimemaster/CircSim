#include "Geometry.h"

Geometry::Geometry()
{
	this->size = 0;
	this->type = DrawType::BUFFER_ONLY;
	this->gpuByteSize = 0;
}

Geometry::Geometry(DrawType type, std::vector<glm::vec2> points)
{
	GLuint vbo = 0;
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

	Buffer buf;
	buf.id = vbo;
	buf.target = GL_ARRAY_BUFFER;
	buf.gpuSize = this->gpuByteSize;

	buffers[VBO] = buf;
}

Geometry::Geometry(DrawType type, float* points, const size_t size)
{
	GLuint vbo = 0;
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

	Buffer buf;
	buf.id = vbo;
	buf.target = GL_ARRAY_BUFFER;
	buf.gpuSize = this->gpuByteSize;

	buffers[VBO] = buf;
}

Geometry::Geometry(DrawType type, float* points, const size_t size, const size_t attributeSize, bool stride, GLenum usage)
{
	GLuint vbo = 0;
	this->size = size / attributeSize;
	this->type = type;
	this->gpuByteSize = size * sizeof(float);

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->gpuByteSize, points, usage);
	glVertexAttribPointer(0, attributeSize, GL_FLOAT, GL_FALSE, attributeSize * sizeof(GLfloat) * stride, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	Buffer buf;
	buf.id = vbo;
	buf.target = GL_ARRAY_BUFFER;
	buf.gpuSize = this->gpuByteSize;

	buffers[VBO] = buf;
}

Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &vao);

	for (auto b : buffers)
	{
		glDeleteBuffers(1, &b.id);
	}
}
