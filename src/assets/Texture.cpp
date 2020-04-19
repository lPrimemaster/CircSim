#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/img_load/stb_image.h"

Texture::Texture(GLenum target, const char* path, bool invert, GLenum internalformat, GLenum format, GLenum type)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(invert);
	unsigned char* image = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	glCreateTextures(target, 1, &buffer.id);
	glBindTexture(target, buffer.id);
	switch (target)
	{
	case GL_TEXTURE_2D:
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		break;
	}

	this->buffer.type = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char) * channels;

	stbi_image_free(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &buffer.id);
}