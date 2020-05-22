#pragma once
#include "../core/ECS.h"
#include "Events.h"
#include "../components/Camera.h"

class InputHandler : public FCS::System
	<	Events::OnMouseMovement<Events::MovementType::ScreenSpace>,
		Events::OnMouseClick, 
		Events::OnMouseScroll, 
		Events::OnFramebufferResize, 
		Events::OnEngineStartup, 
		Events::OnKeyPressed	>
{
public:
	void initialize(FCS::Scene* scene) override {};
	void deinitialize(FCS::Scene* scene) override {};

	void update(FCS::Scene* scene, float deltaTime) override {};

	void onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::ScreenSpace>& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseClick& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnMouseScroll& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnFramebufferResize& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnEngineStartup& event) override;
	void onEvent(FCS::Scene* scene, const Events::OnKeyPressed& event) override;

private:
	void mouseMoveScreen(FCS::Scene* scene, float x, float y, bool active);

	int screen_width = 0;
	int screen_height = 0;

	GLFWwindow* window;

	float global_zoom = 1.0f;
	glm::vec2 global_last_translate;

	struct MouseInput
	{
		glm::vec2 world_space_pos;
		glm::vec2 screen_space_pos;
		int buttons[3];
	}mouseInput;

	FCS_SYSTEM(InputHandler, 100);
};

