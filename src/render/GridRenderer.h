#pragma once
#include "Renderer.h"


class GridRenderer : public Renderer
{
public:
	GridRenderer();
	~GridRenderer();

	void render() override;

	float& getLW();

private:
	Geometry* bg_line;
	float lw = 0.005f;
};

