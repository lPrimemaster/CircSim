#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../util/memtrace.h"

#pragma warning( disable : 6386 )
#pragma warning( disable : 26812 )

struct Buffer
{
	GLuint id = 0;
	GLenum target = 0;
	size_t gpuSize = 0;
};

class Geometry
{
public:
	enum DrawType : int;
	enum BufferType : int;

	Geometry();
	Geometry(DrawType type, std::vector<glm::vec2> points);
	Geometry(DrawType type, float* points, const size_t size);
	Geometry(DrawType type, float* points, const size_t size, const size_t attributeSize, bool stride = false, GLenum usage = GL_STATIC_DRAW);

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

	~Geometry();

public:
	enum DrawType { POINTS = GL_POINTS, LINES = GL_LINES, TRIANGLES = GL_TRIANGLES, TRIANGLE_STRIP = GL_TRIANGLE_STRIP, LINE_STRIP = GL_LINE_STRIP, LINE_LOOP = GL_LINE_LOOP, FAN = GL_TRIANGLE_FAN, BUFFER_ONLY = 0x50 };
	enum BufferType { VBO, VBO1, VBO2, VBO3, EBO, MBT };

public:
	inline static Geometry* getRegisteredGeometry(const std::string& name)
	{
		return rGeometries[name];
	}

	/* TODO : Add a dynamic update for sub sections of the points calling glBufferSubData */
	/* Calling this function will atomatically set the GL_DYNAMIC_DRAW hint flag on for the data in the buffer being changed / updated */
	template<typename T>
	static void updateDynamicGeometry(const std::string& name, std::vector<T> points, BufferType type = VBO);

	template<typename T>
	static void updateDynamicGeometry(const std::string& name, T* points, const size_t size, BufferType type = VBO);

	template<typename T>
	static void updateDynamicSubGeometry(const std::string& name, T* points, const size_t size, const size_t offset, BufferType type = VBO);

	template<typename T>
	static void addBuffer(const std::string& name, std::vector<T> points, BufferType buffer, GLenum target = GL_ARRAY_BUFFER, GLuint vao_bind_index = 0, GLint bind_size = 0, GLenum bind_type = GL_FLOAT, GLuint divisor = 0);

	template<typename T>
	static void addBuffer(const std::string& name, T* points, const size_t size, BufferType buffer, GLenum target = GL_ARRAY_BUFFER, GLuint vao_bind_index = 0, GLint bind_size = 0, GLenum bind_type = GL_FLOAT, GLuint divisor = 0);

	static void registerGeometry(Geometry* g, const std::string& name);
	static void unregisterGeometry(const std::string& name);
	static std::pair<size_t, size_t> getStatistics();

	static void rehash(size_t value);

private:
	template<typename T>
	static void updateDynamicGeometryI(const std::string& name, T* points, const size_t size, BufferType type);
	
	template<typename T>
	static void updateDynamicSubGeometryI(const std::string& name, T* points, const size_t size, const size_t offset, BufferType type);

	template<typename T>
	static void addBufferI(const std::string& name, T* points, const size_t size, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor);

private:
	static std::unordered_map<std::string, Geometry*> rGeometries;

private:
	DrawType type = LINES;
	size_t size = 0;
	size_t gpuByteSize = 0;

private:
	GLuint vao = 0;
	Buffer buffers[MBT];
};

template<typename T>
inline void Geometry::updateDynamicGeometry(const std::string& name, std::vector<T> points, BufferType type)
{
	updateDynamicGeometryI<T>(name, &points.front(), points.size(), type);
}

template<typename T>
inline void Geometry::updateDynamicGeometry(const std::string& name, T* points, const size_t size, BufferType type)
{
	updateDynamicGeometryI<T>(name, points, size, type);
}

template<typename T>
inline void Geometry::updateDynamicSubGeometry(const std::string& name, T* points, const size_t size, const size_t offset, BufferType type)
{
	updateDynamicSubGeometryI<T>(name, points, size, offset, type);
}

template<typename T>
inline void Geometry::addBuffer(const std::string& name, std::vector<T> points, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	addBufferI<T>(name, points.data(), points.size() * sizeof(T), buffer, target, vao_bind_index, bind_size, bind_type, divisor);
}

template<typename T>
inline void Geometry::addBuffer(const std::string& name, T* points, const size_t size, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	addBufferI<T>(name, points, size, buffer, target, vao_bind_index, bind_size, bind_type, divisor);
}

template<typename T>
inline void Geometry::updateDynamicGeometryI(const std::string& name, T* points, const size_t size, BufferType type)
{
	Geometry* geo = getRegisteredGeometry(name);

	if (type == VBO)
	{
		//Assume we are always using xy values (duplets)
		geo->size = size / 2;
	}

	Buffer& namedbuffer = geo->buffers[type];

	size_t batch_size = size * sizeof(T);
	geo->gpuByteSize -= namedbuffer.gpuSize;
	geo->gpuByteSize += batch_size;
	namedbuffer.gpuSize = batch_size;

	glBindBuffer(namedbuffer.target, namedbuffer.id);
	glBufferData(namedbuffer.target, batch_size, points, GL_DYNAMIC_DRAW);
	glBindBuffer(namedbuffer.target, 0);
}

template<typename T>
inline void Geometry::updateDynamicSubGeometryI(const std::string& name, T* points, const size_t size, const size_t offset, BufferType type)
{
	Geometry* geo = getRegisteredGeometry(name);

	if (type == VBO)
	{
		//Assume we are always using xy values (duplets)
		geo->size = size / 2;
	}

	Buffer& namedbuffer = geo->buffers[type];

	size_t batch_size = size * sizeof(T);
	geo->gpuByteSize -= namedbuffer.gpuSize;
	geo->gpuByteSize += batch_size;
	namedbuffer.gpuSize = batch_size;

	glBindBuffer(namedbuffer.target, namedbuffer.id);
	glBufferSubData(namedbuffer.target, offset, batch_size, points);
	glBindBuffer(namedbuffer.target, 0);
}

template<typename T>
inline void Geometry::addBufferI(const std::string& name, T* points, const size_t size, BufferType buffer, GLenum target, GLuint vao_bind_index, GLint bind_size, GLenum bind_type, GLuint divisor)
{
	Geometry* geo = getRegisteredGeometry(name);

	Buffer& namedbuffer = geo->buffers[buffer];

	glCreateBuffers(1, &geo->buffers[buffer].id);
	glBindBuffer(target, namedbuffer.id);
	namedbuffer.target = target;

	namedbuffer.gpuSize = size * sizeof(T);
	geo->gpuByteSize += namedbuffer.gpuSize;

	glBufferData(target, namedbuffer.gpuSize, points, GL_STATIC_DRAW);

	if (vao_bind_index != 0) //Check for colisions maybe == We're all consenting adults here....
	{
		glBindVertexArray(geo->vao);
		glVertexAttribPointer(vao_bind_index, bind_size, bind_type, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(vao_bind_index);
		glVertexAttribDivisor(vao_bind_index, divisor);
		glBindVertexArray(0);
	}
}
