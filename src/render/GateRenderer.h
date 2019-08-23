#pragma once
#include "Renderer.h"
class GateRenderer : public Renderer
{
public:
	GateRenderer();
	~GateRenderer();

	void render() override;
};

