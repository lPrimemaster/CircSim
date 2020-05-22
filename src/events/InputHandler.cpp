#include "InputHandler.h"
#include "../util/math.h"
#include "../util/perf_counter.h"
#include "../components/Sprite.h"
#include "../components/Collider.h"
#include <iostream>

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::ScreenSpace>& event)
{
	// Store mouse coordinates SS
	mouseInput.screen_space_pos.x = event.x;
	mouseInput.screen_space_pos.y = event.y;

	// Store mouse coordinates WS
	mouseInput.world_space_pos = scene->getAllWith<Camera>()[0]->getComponent<Camera>()->screenToWorld(event.x, event.y);

	Events::OnMouseMovement<Events::MovementType::WorldSpace> mme;

	mme.x = mouseInput.world_space_pos.x;
	mme.y = mouseInput.world_space_pos.y;
	mme.window = event.window;

	scene->emit<Events::OnMouseMovement<Events::MovementType::WorldSpace>>(mme);

	// Move screen accordingly
	mouseMoveScreen(scene, event.x, event.y, mouseInput.buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS);
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseClick& event)
{
	mouseInput.buttons[event.button] = event.action;
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseScroll& event)
{
	// Get camera
	auto camera = scene->getAllWith<Camera>()[0]->getComponent<Camera>();

	// Change zoom accordingly
	global_zoom += -event.y;
	if (global_zoom < 1.0f)
	{
		global_zoom = 1.0f;
	}
	camera->view2DInstantScale(1 / (1.0f + global_zoom));

	// Scrolling also afects world position
	// Store mouse coordinates WS
	mouseInput.world_space_pos = camera->screenToWorld(mouseInput.screen_space_pos.x, mouseInput.screen_space_pos.y);

	Events::OnMouseMovement<Events::MovementType::WorldSpace> mme;

	mme.x = mouseInput.world_space_pos.x;
	mme.y = mouseInput.world_space_pos.y;
	mme.window = event.window;

	scene->emit<Events::OnMouseMovement<Events::MovementType::WorldSpace>>(mme);
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnFramebufferResize& event)
{
	screen_width = event.width;
	screen_height = event.height;

	// On Resize update GUI projection
	scene->getAllWith<Camera>()[0]->getComponent<Camera>()->updateScreen(screen_width, screen_height);
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnEngineStartup& event)
{
	screen_width = event.fb_width;
	screen_height = event.fb_height;

	scene->getAllWith<Camera>()[0]->getComponent<Camera>()->updateScreen(screen_width, screen_height);
	window = event.window;

}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnKeyPressed& event)
{
	static unsigned sprt = 0;
	if (event.key == GLFW_KEY_K && event.action == GLFW_PRESS)
	{
		scene->getAllWith<Sprite>()[0]->getComponent<Sprite>()->selectSprite(++sprt % 4);
		printf("sprt % 4 = %u\n", sprt % 4);
	}
}

void InputHandler::mouseMoveScreen(FCS::Scene* scene, float x, float y, bool active)
{
	auto main_cam = scene->getAllWith<Camera>()[0];
	auto cam_component = main_cam->getComponent<Camera>();

	glm::vec2 call_pos = glm::vec2(x, y);

	if (active)
	{
		// Calculate mouse translateIncrement speed on world position
		glm::vec2 delta = global_last_translate - call_pos;

		float weight_x = delta.x * 2.0 / screen_width;
		float weight_y = delta.y * 2.0 / screen_width;

		float abs_scaled = std::abs(global_zoom);

		float translate_speed_x = weight_x * (abs_scaled + 1.0f);
		float translate_speed_y = weight_y * (abs_scaled + 1.0f);

		// Move camera
		cam_component->view2DIncrementTranslation(glm::vec2(-translate_speed_x, translate_speed_y));
	}

	//cam_component->view2DInstant(global_translate, 1 / (1.0f + global_zoom));

	global_last_translate = call_pos;

	// Keep mouse inside window when dragging
	if (active)
	{
		bool over_x = x > (float)screen_width;
		bool over_y = y > (float)screen_height;
		bool under_x = x < 0.0f;
		bool under_y = y < 0.0f;

		bool mchanged = over_x || over_y || under_x || under_y;

		if (mchanged)
		{
			global_last_translate.x = over_x ? 0.0f : under_x ? (float)screen_width  : global_last_translate.x;
			global_last_translate.y = over_y ? 0.0f : under_y ? (float)screen_height : global_last_translate.y;

			glfwSetCursorPos(window, global_last_translate.x, global_last_translate.y);
		}
	}
}
