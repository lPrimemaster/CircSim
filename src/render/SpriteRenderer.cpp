#include "SpriteRenderer.h"

void SpriteRenderer::initialize(FCS::Scene* scene)
{
	quad = Registry::GetAsset<Geometry>("Rectangle");
}

void SpriteRenderer::deinitialize(FCS::Scene* scene)
{
}

void SpriteRenderer::update(FCS::Scene* scene, float deltaTime)
{
	// TODO: this code should move to onCreate and onDestroy Events
	auto to_update = scene->getAllWith<Sprite, Material, Transform>();
	auto cam = scene->getAllWith<Camera>()[0]->getComponent<Camera>();

	for (auto ent : to_update)
	{
		auto mat = ent->getComponent<Material>();
		auto gc = ent->getComponent<Sprite>();
		auto tf = ent->getComponent<Transform>();
		auto p = mat->getProgram();

		if (gc->alphaBlend())
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		p->bind();
		//p->loadVector4f("color", mat->getColor());
		p->loadMatrix4f("PView", cam->getPVMatrix());
		p->loadMatrix4f("Model", tf->getModelMatrix());

		gc->bind();
		quad->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void SpriteRenderer::onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event)
{
}
