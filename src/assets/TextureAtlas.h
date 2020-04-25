#pragma once
#include "Texture.h"
#include "Registry.h"
#include <filesystem>
#include <map>
#include <glm/glm.hpp>
#include <algorithm>

class TextureAtlas : public Asset
{
public:
	TextureAtlas(std::filesystem::path path, glm::ivec2 size, GLint count, GLint root); // Allocate and copy
	TextureAtlas(glm::ivec2 size, GLint count, GLint root); // Allocate only

	void addSubData(unsigned char* img, GLint x, GLint y);

	~TextureAtlas();

public:
	Texture* getTexture() const
	{
		return texture_atlas;
	}

	inline void prepare(unsigned unit = 0) const
	{
		texture_atlas->bind(unit);
	}

	inline void done(unsigned unit = 0) const
	{
		texture_atlas->unbind(unit);
	}

	/*Character& operator[](char c)
	{
		return charmap[c];
	}*/

private:
	GLint count;
	GLint root;
	glm::ivec2 size;
	Texture* texture_atlas;
};

