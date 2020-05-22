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
		GLenum target;
		size_t gpuSize;
	};

	Texture(GLenum target, const char* path, bool invert = true, GLenum internalformat = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLuint depth = 0);
	Texture(GLenum target, unsigned char* img, int width, int height, GLenum internalformat = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLuint depth = 0);
	Texture(GLenum target, int width, int height, GLuint depth, GLenum internalformat = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	void addSubData(unsigned char* img, int width, int height, GLint xoff = 0, GLint yoff = 0, GLint zoff = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
	void addSubData(const char* path, GLint xoff = 0, GLint yoff = 0, GLint zoff = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	inline GLenum getType() const
	{
		return buffer.target;
	}

	inline size_t getSize() const
	{
		return buffer.gpuSize;
	}

	inline void bind(unsigned unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(buffer.target, buffer.id);
	}

	inline void unbind(unsigned unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(buffer.target, NULL);
	}

	~Texture();

private:
	Buffer buffer;
};

