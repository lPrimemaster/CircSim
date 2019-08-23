#include "Renderer.h"
#include "GridRenderer.h"
#include "gui/Gui.h"

GridRenderer::GridRenderer() : Renderer("aa_lines", ADD_GEOM_SHADER)
{
	bg_line = Geometry::getRegisteredGeometry("Line");
}

GridRenderer::~GridRenderer()
{
}

void GridRenderer::render()
{
	p.bind();
	p.loadMatrix4f("PView", pvm);
	p.loadFloat("lineWidth", lw);

	for (auto c : comps)
	{
		p.loadVector4f("color", c->getColor());
		p.loadMatrix4f("Model", c->transform().getTransform());

		bg_line->bind();
		glDrawArraysInstanced(GL_LINES, 0, 2, 1);
	}
}

float& GridRenderer::getLW()
{
	return lw;
}
