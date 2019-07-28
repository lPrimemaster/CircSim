#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma warning( disable : 6386 )

class Geometry
{
public:
	enum DrawType : int;
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

public:
	inline static Geometry* getRegisteredGeometry(const std::string& name)
	{
		return rGeometries[name];
	}
	static void registerGeometry(Geometry* g, const std::string& name);
	static void unregisterGeometry(const std::string& name);
	static void getStatistics();

	static void rehash(size_t value);

private:
	static std::unordered_map<std::string, Geometry*> rGeometries;

private:
	DrawType type = LINES;
	size_t size = 0;
	size_t gpuByteSize = 0;

private:
	GLuint vao = 0;
	GLuint vbo = 0;
};

