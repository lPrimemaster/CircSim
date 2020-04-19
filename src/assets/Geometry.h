#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Asset.h"

#pragma warning( disable : 6386 )
#pragma warning( disable : 26812 )

class Geometry : public Asset
{
public:
	struct Buffer
	{
		GLuint id = 0;
		GLenum target = 0;
		size_t gpuSize = 0;
	};
	enum DrawType : int;
	enum BufferType : int;

	Geometry();
	Geometry(DrawType type, std::vector<glm::vec2> points);
	Geometry(DrawType type, float* points, const size_t size);
	Geometry(DrawType type, float* points, const size_t size, const size_t attributeSize, bool stride = false, GLenum usage = GL_STATIC_DRAW);
	~Geometry();

	inline DrawType getType() const
	{
		return type;
	}

	inline size_t getSize() const
	{
		return size;
	}

	inline void bind() const
	{
		glBindVertexArray(vao);
	}

	inline void bindBuffer(BufferType id) const
	{
		glBindBuffer(buffers[id].target, buffers[id].id);
	}


public:
	enum DrawType { POINTS = GL_POINTS, LINES = GL_LINES, TRIANGLES = GL_TRIANGLES, TRIANGLE_STRIP = GL_TRIANGLE_STRIP, LINE_STRIP = GL_LINE_STRIP, LINE_LOOP = GL_LINE_LOOP, FAN = GL_TRIANGLE_FAN, BUFFER_ONLY = 0x50 };
	enum BufferType { VBO, VBO1, VBO2, VBO3, EBO, MBT };

public:

	/* TODO : Add a dynamic update for sub sections of the points calling glBufferSubData */
	/* Calling this function will atomatically set the GL_DYNAMIC_DRAW hint flag on for the data in the buffer being changed / updated */
	template<typename T>
	void updateDynamicGeometry(std::vector<T> points, BufferType type = VBO);

	template<typename T>
	void updateDynamicGeometry(T* points, const size_t size, BufferType type = VBO);

	template<typename T>
	void updateDynamicSubGeometry(T* points, const size_t size, const size_t offset, BufferType type = VBO);

	template<typename T>
	void addBuffer(std::vector<T> points, BufferType buffer, GLenum target = GL_ARRAY_BUFFER, GLuint vao_bind_index = 0, GLint bind_size = 0, GLenum bind_type = GL_FLOAT, GLuint divisor = 0);

	template<typename T>
	void addBuffer(T* points, const size_t size, BufferType buffer, GLenum target = GL_ARRAY_BUFFER, GLuint vao_bind_index = 0, GLint bind_size = 0, GLenum bind_type = GL_FLOAT, GLuint divisor = 0);

private:
	template<typename T>
	void updateDynamicGeometryI(T* points, const size_t size, BufferType type);
	
	template<typename T>
	void updateDynamicSubGeometryI(T* points, const size_t size, const size_t offset, BufferType type);

	template<typename T>
	void addBufferI(T* points, const size_t size, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor);

private:
	DrawType type = LINES;
	size_t size = 0;
	size_t gpuByteSize = 0;

private:
	GLuint vao = 0;
	Buffer buffers[MBT];
};

template<typename T>
inline void Geometry::updateDynamicGeometry(std::vector<T> points, BufferType type)
{
	updateDynamicGeometryI<T>(&points.front(), points.size(), type);
}

template<typename T>
inline void Geometry::updateDynamicGeometry(T* points, const size_t count, BufferType type)
{
	updateDynamicGeometryI<T>(points, count, type);
}

template<typename T>
inline void Geometry::updateDynamicSubGeometry(T* points, const size_t count, const size_t offset, BufferType type)
{
	updateDynamicSubGeometryI<T>(points, count, offset, type);
}

template<typename T>
inline void Geometry::addBuffer(std::vector<T> points, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	addBufferI<T>(points.data(), points.size() * sizeof(T), buffer, target, vao_bind_index, bind_size, bind_type, divisor);
}

template<typename T>
inline void Geometry::addBuffer(T* points, const size_t count, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	addBufferI<T>(points, count, buffer, target, vao_bind_index, bind_size, bind_type, divisor);
}

template<typename T>
inline void Geometry::updateDynamicGeometryI(T* points, const size_t count, BufferType type)
{
	if (type == VBO)
	{
		//Assume we are always using xy values (duplets)
		this->size = count / 2;
	}

	Buffer& namedbuffer = this->buffers[type];

	size_t batch_size = count * sizeof(T);
	this->gpuByteSize -= namedbuffer.gpuSize;
	this->gpuByteSize += batch_size;
	namedbuffer.gpuSize = batch_size;

	glBindBuffer(namedbuffer.target, namedbuffer.id);
	glBufferData(namedbuffer.target, batch_size, points, GL_DYNAMIC_DRAW);
	glBindBuffer(namedbuffer.target, 0);
}

template<typename T>
inline void Geometry::updateDynamicSubGeometryI(T* points, const size_t count, const size_t offset, BufferType type)
{
	if (type == VBO)
	{
		//Assume we are always using xy values (duplets)
		this->size = count / 2;
	}

	Buffer& namedbuffer = this->buffers[type];

	size_t batch_size = count * sizeof(T);
	this->gpuByteSize -= namedbuffer.gpuSize;
	this->gpuByteSize += batch_size;
	namedbuffer.gpuSize = batch_size;

	glBindBuffer(namedbuffer.target, namedbuffer.id);
	glBufferSubData(namedbuffer.target, offset, batch_size, points);
	glBindBuffer(namedbuffer.target, 0);
}

template<typename T>
inline void Geometry::addBufferI(T* points, const size_t count, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	Buffer& namedbuffer = this->buffers[buffer];

	glCreateBuffers(1, &this->buffers[buffer].id);
	glBindBuffer(target, namedbuffer.id);
	namedbuffer.target = target;

	namedbuffer.gpuSize = count * sizeof(T);
	this->gpuByteSize += namedbuffer.gpuSize;

	glBufferData(target, namedbuffer.gpuSize, points, GL_STATIC_DRAW);

	if (vao_bind_index != 0) //Check for colisions maybe == We're all consenting adults here....
	{
		glBindVertexArray(this->vao);
		glVertexAttribPointer(vao_bind_index, bind_size, bind_type, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(vao_bind_index);
		glVertexAttribDivisor(vao_bind_index, divisor);
		glBindVertexArray(0);
	}
}
