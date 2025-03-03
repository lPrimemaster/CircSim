#pragma once
#include <iostream>

#define GLEW_STATIC

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../util/memtrace.h"

namespace loader
{
	GLuint loadVS_FS(const std::string& name);
	GLuint loadVS_GS_FS(const std::string& name);
}