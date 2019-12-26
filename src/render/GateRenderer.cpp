#include "GateRenderer.h"

GateRenderer::GateRenderer() : Renderer("color") {  }

GateRenderer::~GateRenderer()
{
}

//TODO: Instance render the sections of the gate
//TODO: Create a single gate mesh, not 6 distinct components
void GateRenderer::render()
{
	p.bind();
	p.loadMatrix4f("PView", pvm);
	for (auto c : comps)
	{
		Geometry* geo = Geometry::getRegisteredGeometry(c->geometryName());
		p.loadVector4f("color", c->getColor());
		p.loadMatrix4f("Model", c->transform().getTransform());

		geo->bind();
		glDrawArrays(geo->getType(), 0, geo->getSize());
	}
}