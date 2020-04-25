#include "TextRenderer.h"
#include "../util/perf_counter.h"

void TextRenderer::initialize(FCS::Scene* scene)
{
	geo = Registry::GetAsset<Geometry>("TextRenderGeometry");
	mat = new Material();
	mat->setShader("font", 0);
}

void TextRenderer::deinitialize(FCS::Scene* scene)
{
	delete mat;
}

void TextRenderer::update(FCS::Scene* scene, float deltaTime)
{
	TIMED_BLOCK;
	auto p = mat->getProgram();
	auto to_update = scene->getAllWith<Text, Transform>();
	auto cam = scene->getAllWith<Camera>()[0]->getComponent<Camera>();
	p->bind();
	p->loadMatrix4f("PView", cam->getPVMatrix());
	geo->bind();
	// Batch text and send all at once

	std::vector<GLfloat> drawBatch;
	std::vector<GLfloat> colorAtt;

	glEnable(GL_BLEND);

	for (auto t : to_update)
	{
		auto text = t->getComponent<Text>();
		text->formatText("%s cy/h: %llu", debug_records[0].funcname, debug_records[0].cycles / debug_records[0].hitCount);
		auto tfm = t->getComponent<Transform>();
		auto font = Registry::GetAsset<Charmap>(text->getFontName()); // Assure this doesn't become a problem
		std::string textValue = text->getString();
		auto characters_info = font->getString(textValue);

		glm::vec3 position = tfm->getPosition();
		auto color = text->getColor();

		for (auto c : characters_info)
		{
			GLfloat x = position.x + c.bearing.x /* [ADD] * text scale */;
			GLfloat y = position.y - (c.size.y - c.bearing.y) /* [ADD] * text scale */;

			GLfloat w = c.size.x /* [ADD] * text scale */;
			GLfloat h = c.size.y /* [ADD] * text scale */;

			GLfloat uv[4] = { 
				c.uvCoords.x,			   c.uvCoords.y,
				c.uvCoords.x + c.uvSize.x, c.uvCoords.y + c.uvSize.y 
			};

			std::vector<GLfloat> vertices = {
				x,     y + h,   uv[0], uv[3],
				x,     y,       uv[0], uv[1],
				x + w, y,       uv[2], uv[1],

				x,     y + h,   uv[0], uv[3],
				x + w, y,       uv[2], uv[1],
				x + w, y + h,   uv[2], uv[3]
			};

			position.x += (c.advance >> 6) /* [ADD] * text scale */;

			drawBatch.insert(drawBatch.end(), vertices.begin(), vertices.end());
			colorAtt.push_back(color.r); colorAtt.push_back(color.g); colorAtt.push_back(color.b);
		}
		geo->updateDynamicGeometry(drawBatch, Geometry::VBO);
		geo->updateDynamicGeometry(colorAtt, Geometry::VBO1);

		font->bind();
		glDrawArrays(GL_TRIANGLES, 0, drawBatch.size());

		drawBatch.clear();
		colorAtt.clear();
	}
}

void TextRenderer::onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event)
{
}
