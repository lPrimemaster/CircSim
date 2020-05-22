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
#include "../util/math.h"

class DebugRenderer : public FCS::System
	<
	Events::OnPCounterInitDebugInfo, 
	Events::OnMouseMovement<Events::MovementType::WorldSpace>,
	Events::OnMouseMovement<Events::MovementType::ScreenSpace>
	>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const Events::OnPCounterInitDebugInfo& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::WorldSpace>& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::ScreenSpace>& event) override;

private:
	std::vector<FCS::Handle<FCS::Entity>> debug_time_text;
	std::vector<FCS::Handle<FCS::Entity>> debug_pos_text;
	int frame_debug_text = 0;

	glm::vec2 mouse_position_ss;
	glm::vec2 mouse_position_ws;

	FCS_SYSTEM(DebugRenderer, 0); // Highest priority - last call
};
