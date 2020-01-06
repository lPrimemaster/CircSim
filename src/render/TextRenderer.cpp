#include "TextRenderer.h"
#include "chars/CharAtlas.h"

TextRenderer::TextRenderer() : Renderer("font")
{
	Geometry::registerGeometry(new Geometry(Geometry::TRIANGLES, nullptr, 24, 4, false, GL_DYNAMIC_DRAW), "textQuad");
	quad_geom = Geometry::getRegisteredGeometry("textQuad");
}

TextRenderer::~TextRenderer()
{
	Geometry::unregisterGeometry("textQuad");
}

void TextRenderer::render()
{
	p.bind();
	p.loadMatrix4f("proj", pvm);
	quad_geom->bind();

	for (auto t : text_batch)
	{
		p.loadVector3f("textColor", t.color);

		std::string::const_iterator ci;
		auto cAtlas = CharAtlas::getRegisteredCharAtlas(t.font);
		float text_advance = t.x;

		for (ci = t.text.cbegin(); ci != t.text.cend(); ci++)
		{
			Character ch = (*cAtlas)[*ci];

			GLfloat xpos = text_advance + ch.bearing.x * t.scale;
			GLfloat ypos = t.y - (ch.size.y - ch.bearing.y) * t.scale;

			GLfloat w = ch.size.x * t.scale;
			GLfloat h = ch.size.y * t.scale;

			GLfloat vertices[24] = {
				xpos,     ypos + h,   0.0, 0.0 ,
				xpos,     ypos,       0.0, 1.0 ,
				xpos + w, ypos,       1.0, 1.0 ,

				xpos,     ypos + h,   0.0, 0.0 ,
				xpos + w, ypos,       1.0, 1.0 ,
				xpos + w, ypos + h,   1.0, 0.0 
			};

			ch.texture->bind();
			Geometry::updateDynamicSubGeometry("textQuad", vertices, 24, 0); //FIX: this is a overhead

			glDrawArrays(GL_TRIANGLES, 0, 6);
			text_advance += (ch.advance >> 6) * t.scale;
		}
	}
}

void TextRenderer::setDefaultFont(std::string font)
{
	default_font = font;
}

void TextRenderer::addText(std::string text, glm::vec2 position, glm::vec3 color, float scale, std::string font)
{
	Text to_add;
	to_add.font = font == "" ? default_font : font;
	to_add.color = color;
	to_add.scale = scale;
	to_add.x = position.x;
	to_add.y = position.y;
	to_add.text = text;

	text_batch.push_back(to_add);
}
