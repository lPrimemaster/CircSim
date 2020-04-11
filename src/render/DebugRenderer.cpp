#include "DebugRenderer.h"

DebugRenderer::DebugRenderer() : Renderer("debug", ADD_GEOM_SHADER) {  }

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::render()
{
	p.bind();
	p.loadMatrix4f("PView", pvm);

	//Using geometry shader instead
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto c : comps)
	{
		Geometry* geo = Geometry::getRegisteredGeometry(c->geometryName()); //FIX: this is a overhead
		p.loadVector4f("color", c->getColor());
		p.loadMatrix4f("Model", c->transform().getTransform());

		geo->bind();
		glDrawArrays(geo->getType(), 0, geo->getSize());
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
