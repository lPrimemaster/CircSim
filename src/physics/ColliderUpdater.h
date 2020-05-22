#pragma once
#include <iostream>
#include "../core/ECS.h"
#include "../events/Events.h"

#include "../components/Collider.h"
#include "../components/Sprite.h"
#include "../components/Camera.h"

class ColliderUpdater : public FCS::System<Events::OnMouseMovement<Events::MovementType::WorldSpace>>
{
public:
	void initialize(FCS::Scene* scene) override {};
	void deinitialize(FCS::Scene* scene) override {};

	void update(FCS::Scene* scene, float delta) override;

	void onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::WorldSpace>& event) override;

private:
	FCS_SYSTEM(ColliderUpdater, 99);
};

