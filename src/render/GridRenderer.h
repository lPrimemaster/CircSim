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

	//Make sure no extra components get in this renderer
	void push(GraphicComponent* c) override {}
	void pushList(GraphicComponent** list, size_t size) override {}

	void pop(GraphicComponent* c) override {}
	void popList(GraphicComponent** list, size_t size) override {}

private:
	void generateGrid();

private:
	float lw = 0.005f;

	float* offsets = nullptr;
	unsigned totalGenLines = 20; //Ensure 20 lines at least
};

