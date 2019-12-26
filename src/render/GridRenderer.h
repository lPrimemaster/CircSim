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
	void push(Component* c) override {}
	void pushList(Component** list, size_t size) override {}

	void pop(Component* c) override {}
	void popList(Component** list, size_t size) override {}

private:
	void generateGrid();

private:
	float lw = 0.005f;

	float* offsets = nullptr;
	unsigned totalGenLines = 20; //Ensure 20 lines at least
};

