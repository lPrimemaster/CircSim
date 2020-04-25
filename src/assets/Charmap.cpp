#include "Charmap.h"
#include "../util/math.h"

constexpr unsigned int NCHARS = 128;
constexpr unsigned int WCHARS = 16;

Charmap::Charmap(std::filesystem::path path, FT_UInt height)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, path.string().c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	std::string font_name = path.filename().string();

	FT_Set_Pixel_Sizes(face, 0, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Its ok not to pack the chars to a maximum
	unsigned char* glyphPixels[WCHARS][NCHARS / WCHARS];

	unsigned int maxWidth = 0;
	unsigned int maxHeight = 0;

	for (GLubyte cy = 0; cy < NCHARS / WCHARS; cy++)
	{
		for (GLubyte cx = 0; cx < WCHARS; cx++)
		{
			GLint x = (GLint)cx;
			GLint y = (GLint)cy;
			GLchar c = (GLchar)(cy * WCHARS + cx);


			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			FT_Bitmap b = face->glyph->bitmap;
			GLuint size = b.width * b.rows;
			maxWidth = math::swapMax(maxWidth, b.width);
			maxHeight = math::swapMax(maxHeight, b.rows);
			glyphPixels[x][y] = (unsigned char*)malloc(sizeof(unsigned char) * size);
			if(glyphPixels[x][y] != nullptr)
				std::memcpy(glyphPixels[x][y], b.buffer, size);
			math::flipImageVertically(glyphPixels[x][y], b.width, b.rows, 1);
			Character ch;
			ch.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			ch.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			ch.advance = face->glyph->advance.x;
			charmap.emplace(c, ch);
		}
	}

	texture = new Texture(GL_TEXTURE_2D, maxWidth * WCHARS, maxHeight * NCHARS / WCHARS, 0, GL_R8, GL_RED);

	for (GLubyte cy = 0; cy < NCHARS / WCHARS; cy++)
	{
		for (GLubyte cx = 0; cx < WCHARS; cx++)
		{
			GLint x = (GLint)cx;
			GLint y = (GLint)cy;
			GLchar c = (GLchar)(cy * WCHARS + cx);

			GLint xoff = x * maxWidth;
			GLint yoff = y * maxHeight;

			glm::ivec2 size = charmap[c].size;
			// TODO: opengl invert y uvCoords
			charmap[c].uvCoords = glm::vec2(xoff / (float)(WCHARS * maxWidth), yoff / (float)((NCHARS / WCHARS) * maxHeight));
			charmap[c].uvSize = glm::vec2(size.x / (float)(WCHARS * maxWidth), size.y / (float)((NCHARS / WCHARS) * maxHeight));
			texture->addSubData(glyphPixels[x][y], size.x, size.y, xoff, yoff, 0, GL_RED);
			free(glyphPixels[x][y]);
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

Charmap::~Charmap()
{
	delete texture;
}

std::vector<Character> Charmap::getString(const std::string& str)
{
	std::vector<Character> chars;
	for (auto c : str)
	{
		chars.push_back(charmap[c]);
	}
	return chars;
}

Character Charmap::getCharacter(const char& c)
{
	return charmap[c];
}
