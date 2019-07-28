#pragma once
#include <iostream>
#include "../util/loader.h"

/* This class could use inheritance for multiple program capabilities, but meh */

class Program
{
public:
	Program(const std::string& shaderFileName);
	~Program();

	void bind() const;
	void unbind() const;
	GLuint getID() const;

	void loadInt(const char* name, int value);
	void loadUInt(const char* name, unsigned value);
	void loadFloat(const char* name, float value);
	void loadVector2f(const char* name, const glm::vec2 value);
	void loadVector3f(const char* name, const glm::vec3 value);
	void loadVector4f(const char* name, const glm::vec4 value);
	void loadMatrix4f(const char* name, const glm::mat4 value);

private:
	GLuint programID = 0;
};

