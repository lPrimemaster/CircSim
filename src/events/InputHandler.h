#pragma once
#include "../core/ECS.h"
#include "Events.h"
#include "../components/Camera.h"

class InputHandler : public FCS::System
	<Events::OnMouseMovement, Events::OnMouseClick, Events::OnMouseScroll, Events::OnFramebufferResize, Events::OnEngineStartup>
{
public:
	void initialize(FCS::Scene* scene) override {};
	void deinitialize(FCS::Scene* scene) override {};

	void update(FCS::Scene* scene, float deltaTime) override {};

	void onEvent(FCS::Scene* scene, const Events::OnMouseMovement& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseClick& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseScroll& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnFramebufferResize& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnEngineStartup& event) override;

private:
	void mouseMoveScreen(FCS::Scene* scene, const Events::OnMouseMovement& event, bool active);

	int screen_width = 0;
	int screen_height = 0;

	int screen_effective_width = 0;
	int screen_effective_height = 0;

	float global_zoom = 1.0f;
	glm::vec2 global_translate;
	glm::vec2 global_last_translate;

	FCS_SYSTEM(InputHandler, 100);
};

