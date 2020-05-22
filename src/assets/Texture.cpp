#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/img_load/stb_image.h"

Texture::Texture(GLenum target, const char* path, bool invert, GLenum internalformat, GLenum format, GLenum type, GLuint depth)
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
	case GL_TEXTURE_2D_ARRAY:
		glTexImage3D(target, 0, internalformat, width, height, depth, 0, format, type, image);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	default:
		break;
	}

	this->buffer.target = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char) * channels;

	stbi_image_free(image);

	std::cout << "Loaded Texture [" << width << "x" << height << "] from " << path << std::endl;
}

Texture::Texture(GLenum target, unsigned char* img, int width, int height, GLenum internalformat, GLenum format, GLenum type, GLuint depth)
{
	glCreateTextures(target, 1, &buffer.id);
	glBindTexture(target, buffer.id);
	switch (target)
	{
	case GL_TEXTURE_2D:
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case GL_TEXTURE_2D_ARRAY:
		glTexImage3D(target, 0, internalformat, width, height, depth, 0, format, type, img);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	default:
		break;
	}

	this->buffer.target = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char) * 4; // Assume 4 channels here, because why not

	std::cout << "Loaded Texture [" << width << "x" << height << "] from memory " << static_cast<void*>(img) << std::endl;
}

Texture::Texture(GLenum target, int width, int height, GLuint depth, GLenum internalformat, GLenum format, GLenum type)
{
	glCreateTextures(target, 1, &buffer.id);
	glBindTexture(target, buffer.id);
	switch (target)
	{
	case GL_TEXTURE_2D:
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case GL_TEXTURE_2D_ARRAY:
		glTexStorage3D(target, 1, internalformat, width, height, depth);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	default:
		break;
	}

	this->buffer.target = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char) * 4; // Assume 4 channels here, because why not

	std::cout << "Created Empty Texture [" << this->buffer.id << "] [" << width << "x" << height << "] in memory" << std::endl;
}

void Texture::addSubData(unsigned char* img, int width, int height, GLint xoff, GLint yoff, GLint zoff, GLenum format, GLenum type)
{
	glBindTexture(buffer.target, buffer.id);
	switch (buffer.target)
	{
	case GL_TEXTURE_2D:
		glTexSubImage2D(buffer.target, 0, xoff, yoff, width, height, format, type, img);
		break;
	case GL_TEXTURE_2D_ARRAY:
		glTexSubImage3D(buffer.target, 0, xoff, yoff, zoff, width, height, 1, format, type, img);
	default:
		break;
	}

	std::cout << "Loaded SubTexture [" << this->buffer.id << "] [" << width << "x" << height << "] from memory " << static_cast<void*>(img) << std::endl;
}

void Texture::addSubData(const char* path, GLint xoff, GLint yoff, GLint zoff, GLenum format, GLenum type)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	addSubData(image, width, height, xoff, yoff, zoff, format, type);

	stbi_image_free(image);

	std::cout << "Loaded SubTexture [" << this->buffer.id << "] [" << width << "x" << height << "] from " << path << std::endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &buffer.id);
}