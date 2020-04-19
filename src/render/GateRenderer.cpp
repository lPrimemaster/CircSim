#include "GateRenderer.h"
#include "../features/ChunkManager.h"

GateRenderer::GateRenderer() : Renderer("color") {  }

GateRenderer::~GateRenderer()
{
}

//TODO: Instance render the sections of the gate
//TODO: Create a single gate mesh, not 6 distinct components
void GateRenderer::render()
{
	//Render all gates
	glEnable(GL_BLEND);

	p.bind();
	p.loadMatrix4f("PView", pvm);
	for (auto c : comps)
	{
		Geometry* geo = Geometry::getRegisteredGeometry(c->geometryName()); //FIX: this is a overhead
		p.loadVector4f("color", c->getColor());
		p.loadMatrix4f("Model", c->transform().getModelMatrix());

		geo->bind();
		glDrawArrays(geo->getType(), 0, geo->getSize());
	}

	//Render the connectors -- this will do for now
	auto lChunks = ChunkManager::getAllLoadedChunks();

	Geometry* geo = Geometry::getRegisteredGeometry("Circle");
	for (auto c : lChunks)
	{
		auto connectors = c->getConnectorsList();
		for (auto con : connectors)
		{
			p.loadMatrix4f("Model", glm::translate(glm::mat4(1.0f), glm::vec3(con->position, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02f)));
			p.loadVector4f("color", glm::vec4(0, 204, 255, 127) / 255.0f);

			geo->bind();
			glDrawArrays(geo->getType(), 0, geo->getSize());
		}
	}

	glDisable(GL_BLEND);
}