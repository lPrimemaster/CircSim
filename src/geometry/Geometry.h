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

struct Buffer
{
	GLuint id;
	GLenum target;
	size_t gpuSize;
};

class Geometry
{
public:
	enum DrawType : int;
	enum BufferType : int;
	Geometry(DrawType type, std::vector<glm::vec2> points);
	Geometry(DrawType type, float* points, const size_t size);

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

	~Geometry();

public:
	enum DrawType { POINTS = GL_POINTS, LINES = GL_LINES, TRIANGLES = GL_TRIANGLES, LINE_STRIP = GL_TRIANGLE_STRIP, LINE_LOOP = GL_LINE_LOOP, FAN = GL_TRIANGLE_FAN };
	enum BufferType { VBO, MBT };

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
	static void addBuffer(const std::string& name, std::vector<T> points, const std::string& buffer, GLenum target = GL_ARRAY_BUFFER);

	template<typename T>
	static void addBuffer(const std::string& name, T* points, const size_t size, const std::string& buffer, GLenum target = GL_ARRAY_BUFFER);

	static void registerGeometry(Geometry* g, const std::string& name);
	static void unregisterGeometry(const std::string& name);
	static std::pair<size_t, size_t> getStatistics();

	static void rehash(size_t value);

private:
	template<typename T>
	static void updateDynamicGeometryI(const std::string& name, T* points, const size_t size, BufferType type);

	template<typename T>
	static void addBufferI(const std::string& name, T* points, const size_t size, const std::string& buffer, GLenum target);

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
inline void Geometry::addBuffer(const std::string& name, std::vector<T> points, const std::string& buffer, GLenum target)
{
	//TODO
}

template<typename T>
inline void Geometry::addBuffer(const std::string& name, T* points, const size_t size, const std::string& buffer, GLenum target)
{
	//TODO
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
}

template<typename T>
inline void Geometry::addBufferI(const std::string& name, T* points, const size_t size, const std::string& buffer, GLenum target)
{
	//TODO
}
