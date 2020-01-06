#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/img_load/stb_image.h"

std::unordered_map<std::string, Texture*> Texture::rTextures;

Texture::Texture(TextureType target, std::filesystem::path path, bool invert, GLenum internalformat, GLenum format, GLenum type)
{
	const char* full_path = path.string().c_str();
	int width, height, channels;
	stbi_set_flip_vertically_on_load(invert);
	unsigned char* image = stbi_load(full_path, &width, &height, &channels, STBI_rgb);

	glCreateTextures(target, 1, &buffer.id);
	glBindTexture(target, buffer.id);
	switch (target)
	{
	case Texture::TEX_2D:
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, image);
		break;
	default:
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->type = target;
	this->buffer.type = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char) * channels;
	this->gpuByteSize = this->buffer.gpuSize;

	stbi_image_free(image);
}

Texture::Texture(TextureType target, unsigned char* bytes, GLuint width, GLuint height, GLenum internalformat, GLenum format, GLenum type)
{
	glCreateTextures(target, 1, &buffer.id);
	glBindTexture(target, buffer.id);
	switch (target)
	{
	case Texture::TEX_2D:
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, bytes);
		break;
	default:
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->type = target;
	this->buffer.type = target;
	this->buffer.gpuSize = width * height * sizeof(unsigned char);
	this->gpuByteSize = this->buffer.gpuSize;
}

Texture::~Texture()
{
	glDeleteTextures(1, &buffer.id);
}

void Texture::registerTexture(Texture* t, const std::string& name)
{
	rTextures.emplace(name, t);
}

void Texture::unregisterTexture(const std::string& name)
{
	delete rTextures[name];
}

std::pair<size_t, size_t> Texture::getStatistics()
{
	size_t bCount = rTextures.bucket_count();
	size_t tbSize = 0;
	for (auto t : rTextures)
	{
		tbSize += t.second->gpuByteSize;
	}

	return std::make_pair(bCount, tbSize);
}

void Texture::rehash(size_t value)
{
	rTextures.rehash(value);
}
