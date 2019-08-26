#pragma once
#include "Renderer.h"


class GridRenderer : public Renderer
{
public:
	GridRenderer();
	~GridRenderer();

	void render() override;

	float& getLW();

	unsigned updateGrid(float zscale, glm::vec2 tvec, glm::vec2 sdim);

private:
	void generateGrid();

private:
	float lw = 0.005f;

	std::vector<glm::vec2> offsets;
	unsigned totalGenLines = 0;
};

