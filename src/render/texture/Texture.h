#pragma once
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <GL/glew.h>

struct TextureBuffer
{
	GLuint id;
	GLenum type;
	size_t gpuSize;
};

#pragma warning( disable : 26812 )

class Texture
{
public:
	enum TextureType : int;

	Texture(TextureType target, std::filesystem::path path, bool invert = true, GLenum internalformat = GL_RGB8, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE);
	Texture(TextureType target, unsigned char* bytes, GLuint width, GLuint height, GLenum internalformat, GLenum format, GLenum type);
	
	inline TextureType getType() const
	{
		return type;
	}

	inline size_t getSize() const
	{
		return gpuByteSize;
	}

	inline void bind(unsigned unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(type, buffer.id);
	}
	
	~Texture();

public:
	enum TextureType { TEX_2D = GL_TEXTURE_2D } type;

public:
	inline static Texture* getRegisteredTexture(const std::string& name)
	{
		return rTextures[name];
	}

	static void registerTexture(Texture* t, const std::string& name);
	static void unregisterTexture(const std::string& name);
	static std::pair<size_t, size_t> getStatistics();

	static void rehash(size_t value);


private:
	TextureBuffer buffer;
	size_t gpuByteSize = 0;

private:
	static std::unordered_map<std::string, Texture*> rTextures;
};

