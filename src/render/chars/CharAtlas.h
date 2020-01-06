#pragma once
#include "../texture/Texture.h"
#include <map>
#include <glm/glm.hpp>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	Texture* texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class CharAtlas
{
public:
	CharAtlas(std::filesystem::path path, FT_UInt height = 48);
	~CharAtlas();

public:
	static void registerCharAtlas(CharAtlas* c, const std::string& name);
	static void unregisterCharAtlas(const std::string& name);

	static void rehash(size_t value);

	Character& operator[](char c)
	{
		return charmap[c];
	}

	inline size_t getSize() const
	{
		size_t byteSum = 0;
		for (auto c : charmap)
		{
			byteSum += c.second.texture->getSize();
		}
		return byteSum;
	}

public:
	inline static CharAtlas* getRegisteredCharAtlas(const std::string& name)
	{
		return rCAtlas[name];
	}

private:
	static std::unordered_map<std::string, CharAtlas*> rCAtlas;
	std::map<GLchar, Character> charmap;
	std::string font_name;
};

