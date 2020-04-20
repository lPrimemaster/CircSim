#pragma once
#include <iostream>
#include <unordered_map>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Asset.h"

class Texture : public Asset
{
public:
	struct Buffer
	{
		GLuint id;
		GLenum type;
		size_t gpuSize;
	};

	Texture(GLenum target, const char* path, bool invert = true, GLenum internalformat = GL_RGB8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	inline GLenum getType() const
	{
		return buffer.type;
	}

	inline size_t getSize() const
	{
		return buffer.gpuSize;
	}

	inline void bind(unsigned unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(buffer.type, buffer.id);
	}

	inline void unbind(unsigned unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(buffer.type, NULL);
	}

	~Texture();

private:
	Buffer buffer;
};

