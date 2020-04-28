#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../components/Material.h"
#include "../components/Camera.h"
#include "../assets/Registry.h"
#include "../events/Events.h"

class DebugRenderer : public FCS::System<Events::OnPCounterInitDebugInfo>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const Events::OnPCounterInitDebugInfo& event) override;

private:
	std::vector<FCS::Handle<FCS::Entity>> debug_text;
	int frame_debug_text = 0;

	FCS_SYSTEM(DebugRenderer, 0); // Highest priority - last call
};
