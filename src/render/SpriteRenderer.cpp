#include "SpriteRenderer.h"
#include "../util/perf_counter.h"

void SpriteRenderer::initialize(FCS::Scene* scene)
{
	quad = Registry::GetAsset<Geometry>("Rectangle");
	mat = new Material();
	mat->setShader("color", 0b0000);
}

void SpriteRenderer::deinitialize(FCS::Scene* scene)
{
	delete mat;
}

void SpriteRenderer::update(FCS::Scene* scene, float deltaTime)
{
	TIMED_BLOCK;
	// TODO: this code should move to onCreate and onDestroy Events
	auto to_update = scene->getAllWith<Sprite, Transform>();
	auto cam = scene->getAllWith<Camera>()[0]->getComponent<Camera>();
	auto p = mat->getProgram();

	for (auto ent : to_update)
	{
		auto gc = ent->getComponent<Sprite>();
		auto tf = ent->getComponent<Transform>();

		if (gc->hasAlphaBlend())
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		p->bind();
		p->loadMatrix4f("PView", cam->getPVMatrix());
		p->loadMatrix4f("Model", tf->getModelMatrix());

		int wm = gc->getWrapMode();

		p->loadInt("WrapMode", wm);

		if(gc->isAnimated())
			p->loadUInt("sprite_layer", gc->getSpriteActiveLayer());

		//TODO: Move all this to the sprite component
		if (wm)
		{
			glm::vec2 size = tf->getScale() * 2.0f;

			float slice = 140.0f;
			float borderScale = 0.3f;

			p->loadVector2f("SliceSize", slice / size * borderScale);
			p->loadVector2f("SliceBorder", slice / glm::vec2(820.0f));
		}

		gc->prepare();
		quad->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		gc->done();
	}
}

void SpriteRenderer::onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event)
{
}
