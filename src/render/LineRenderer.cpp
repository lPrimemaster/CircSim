#include "LineRenderer.h"
#include "../util/perf_counter.h"

void LineRenderer::initialize(FCS::Scene* scene)
{
	line = Registry::GetAsset<Geometry>("Line");
	mat = new Material();
	mat->setShader("aa_lines", ADD_GEOM_SHADER);
}

void LineRenderer::deinitialize(FCS::Scene* scene)
{
	delete mat;
}

void LineRenderer::update(FCS::Scene* scene, float deltaTime)
{
	TIMED_BLOCK;
	auto to_update = scene->getAllWith<Line, Transform>();
	auto cam = scene->getAllWith<Camera>()[0]->getComponent<Camera>(); // Remove magic numbers getAllWith<Camera>()[0 <-- here]

	glDisable(GL_BLEND);
	auto p = mat->getProgram();
	p->bind();
	p->loadVector4f("color", mat->getColor()); // Performance issues here [ for some reason, idk what to do anymore =[ ]
	p->loadMatrix4f("PView", cam->getPVMatrix());
	line->bind();

	for (auto ent : to_update)
	{
		auto ln = ent->getComponent<Line>();
		auto tf = ent->getComponent<Transform>();

		p->loadMatrix4f("Model", tf->getModelMatrix());

		glDrawArrays(GL_LINES, 0, 2);
	}
}

void LineRenderer::onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event)
{
}
