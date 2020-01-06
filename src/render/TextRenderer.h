#pragma once
#include "Renderer.h"
#include "GLWrapper.h"

struct Text
{
	std::string text;
	std::string font;
	glm::vec3 color = glm::vec3(1.0f);
	float scale = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
};

class TextRenderer : public Renderer
{
public:
	TextRenderer();
	~TextRenderer();

	void render() override;

	void setDefaultFont(std::string font);
	void addText(std::string text, glm::vec2 position, glm::vec3 color = glm::vec3(1.0f), float scale = 1.0f, std::string font = "");

	//Make sure no extra components get in this renderer
	void push(Component* c) override {}
	void pushList(Component** list, size_t size) override {}

	void pop(Component* c) override {}
	void popList(Component** list, size_t size) override {}

private:
	glm::mat4 project;
	std::vector<Text> text_batch;
	Geometry* quad_geom = nullptr;
	std::string default_font;
};

