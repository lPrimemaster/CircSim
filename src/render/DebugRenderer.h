#pragma once
#include "Renderer.h"
class DebugRenderer : public Renderer
{
public:
	DebugRenderer();
	~DebugRenderer();

	void render() override;
};

