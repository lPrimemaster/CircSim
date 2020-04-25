#include "TextureAtlas.h"
#include <numeric>

#include "../lib/img_load/stb_image.h"

TextureAtlas::TextureAtlas(std::filesystem::path path, glm::ivec2 size, GLint count, GLint root)
{
	this->count = count;
	this->root = root;
	this->size = size;

	GLint m = count / root;
	glm::ivec2 dim;

	dim.x = size.x * std::min(count, root);
	dim.y = size.y * std::max(1, m);

	texture_atlas = new Texture(GL_TEXTURE_2D, dim.x, dim.y, 0, GL_RGB8);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* image = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

	GLint offsetX = 0;
	GLint offsetY = 0;

	for (GLint i = 0; i < count; i++)
	{
		GLint xoff = (i % root) * size.x;
		GLint yoff = (i / root) * size.y;
		unsigned char* si = image + (channels * (yoff * width + xoff));
		texture_atlas->addSubData(si, size.x, size.y, xoff, yoff, 0);
	}

	stbi_image_free(image);
}

TextureAtlas::TextureAtlas(glm::ivec2 size, GLint count, GLint root)
{
	this->count = count;
	this->root = root;
	this->size = size;

	GLint m = count / root;
	glm::ivec2 dim;

	dim.x = size.x * std::min(count, root);
	dim.y = size.y * std::max(1, m);

	texture_atlas = new Texture(GL_TEXTURE_2D, dim.x, dim.y, 0, GL_RGB8);
}

void TextureAtlas::addSubData(unsigned char* img, GLint x, GLint y)
{
	GLint xoff = (x % root) * size.x;
	GLint yoff = (y / root) * size.y;
	texture_atlas->addSubData(img, size.x, size.y, xoff, yoff, 0);
}

TextureAtlas::~TextureAtlas()
{
	delete texture_atlas;
}
