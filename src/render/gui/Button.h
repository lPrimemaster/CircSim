#pragma once
#include "../../geometry/Geometry.h"
#include "../GuiRenderer.h"
#include "../texture/Texture.h"

class Button
{
public:
	Button();
	~Button();

protected:
	GuiWindow* parent = nullptr;

protected:
	glm::vec2 localPosition;
	glm::vec2 extents;

private:
	Texture* drawable_texture = nullptr;
	GraphicComponent* quad = nullptr;
};

