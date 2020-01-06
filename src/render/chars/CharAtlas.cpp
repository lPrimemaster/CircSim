#include "CharAtlas.h"

std::unordered_map<std::string, CharAtlas*> CharAtlas::rCAtlas;

CharAtlas::CharAtlas(std::filesystem::path path, FT_UInt height)
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

	FT_Set_Pixel_Sizes(face, 0, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	font_name = path.filename().string();

	for (GLubyte c = 0; c < 128; c++)
	{

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		FT_Bitmap b = face->glyph->bitmap;

		Texture* t = new Texture(Texture::TEX_2D, b.buffer, b.width, b.rows, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
		Texture::registerTexture(t, font_name + "_" + static_cast<char>(c));

		t->bind();

		Character ch;
		ch.texture = t;
		ch.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		ch.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		ch.advance = face->glyph->advance.x;

		charmap.insert(std::pair<GLchar, Character>(c, ch));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

CharAtlas::~CharAtlas()
{
	for (GLubyte c = 0; c < 128; c++)
	{
		Texture::unregisterTexture(font_name + "_" + static_cast<char>(c));
	}
}

void CharAtlas::registerCharAtlas(CharAtlas* c, const std::string& name)
{
	rCAtlas.emplace(name, c);
}

void CharAtlas::unregisterCharAtlas(const std::string& name)
{
	delete rCAtlas[name];
}

void CharAtlas::rehash(size_t value)
{
	rCAtlas.rehash(value);
}
