#pragma once
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Registry.h"
#include "Texture.h"
struct Character
{
	glm::vec2 uvCoords;
	glm::vec2 uvSize;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Charmap : public Asset
{
public:
	Charmap(std::filesystem::path path, FT_UInt height = 48);
	~Charmap();

	Character getCharacter(const char& c);
	std::vector<Character> getString(const std::string& str);

	inline void bind(unsigned unit = 0) const
	{
		texture->bind(unit);
	}

	inline void unbind(unsigned unit = 0) const
	{
		texture->unbind(unit);
	}

private:
	Texture* texture;
	std::map<GLchar, Character> charmap;
};

