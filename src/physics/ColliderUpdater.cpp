#include "ColliderUpdater.h"
#include "../util/perf_counter.h"

void ColliderUpdater::update(FCS::Scene* scene, float delta)
{

}

void ColliderUpdater::onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::WorldSpace>& event)
{
	auto colliders = scene->getAllWith<Collider>();
	auto camera = scene->getAllWith<Camera>()[0]->getComponent<Camera>();

	for (auto c : colliders)
	{
		auto comp = c->getComponent<Collider>();
		float x = math::map<float>(event.x, 0.0f, 1280.0f, -1.0f, 1.0f);
		float y = math::map<float>(event.y, 0.0f, 720.0f, -1.0f, 1.0f);
		//glm::vec2 wspace_mouse = glm::inverse(camera->getPVMatrix()) * glm::vec4(x, y, 1.0f, 1.0f);
		if (comp->bounding.intersect(math::screenToWorld(glm::vec2(1280, 720), glm::vec2(event.x, event.y), glm::inverse(camera->getPVMatrix()))))
		{
			auto sprite = c->getComponent<Sprite>();
			sprite->addTexture<Sprite::DIFFUSE>("NotGate_On_Hover");
		}
	}
}
