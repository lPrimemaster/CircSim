#pragma once
#include "Renderer.h"
#include "TextRenderer.h"
#include "../geometry/Geometry.h"

#define GUI_LOCK_TOP	0b0001
#define GUI_LOCK_RIGHT  0b0010

struct GuiWindow
{
	friend class GuiRenderer;

	//Main properties
	glm::vec2 center_pos = glm::vec2(1200.0f, 300.0f);
	glm::vec2 extents = glm::vec2(100.0f, 500.0f);
	glm::vec3 background_color = glm::vec3(110, 19, 3) / 255.0f;
	float background_alpha = 0.5f;

	//Border properties
	float border_thickness = 10.0f;
	glm::vec3 border_color = glm::vec3(240, 124, 74) / 255.0f;
	float border_alpha = 1.0f;

private:
	Geometry* geometry = nullptr;
	float locked_positions[2];
};

class GuiRenderer : public Renderer
{
public:
	GuiRenderer();
	~GuiRenderer();

	typedef int LockType;

	void render() override;
	void setPVMatrix(const glm::mat4& pcm) override;

	void addWindow(GuiWindow* w);
	void adjustWindows(LockType lock);

	//Make sure no extra components get in this renderer
	void push(GraphicComponent* c) override {}
	void pushList(GraphicComponent** list, size_t size) override {}

	void pop(GraphicComponent* c) override {}
	void popList(GraphicComponent** list, size_t size) override {}

private:
	std::vector<GuiWindow*> windows;
	TextRenderer text_renderer;
	Geometry* gui_geom = nullptr;
};

