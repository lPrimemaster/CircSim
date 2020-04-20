#include "InputHandler.h"
#include "../util/math.h"
#include "../util/perf_counter.h"
#include <iostream>

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseMovement& event)
{
	bool right_click = glfwGetMouseButton(event.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	
	// Move screen
	mouseMoveScreen(scene, event, right_click);
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseClick& event)
{
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnMouseScroll& event)
{
	global_zoom += -event.y * 10.0f;
	if (global_zoom < 1.0f) global_zoom = 1.0f;

	float w_ammt = global_zoom * ((float)screen_width / screen_height);
	float h_ammt = global_zoom;
	int vw = screen_width + w_ammt;
	int vh = screen_height + h_ammt;

	screen_effective_width = vw + w_ammt;
	screen_effective_height = vh + h_ammt;

	// Handle scene zoom (no smooth)
	auto main_cam = scene->getAllWith<Camera>()[0];
	auto cam_component = main_cam->getComponent<Camera>();
	cam_component->project(glm::ortho<float>(-w_ammt, vw, -h_ammt, vh, 0.1f, 100.0f));
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnFramebufferResize& event)
{
	screen_height = event.height;
	screen_width = event.width;
}

void InputHandler::onEvent(FCS::Scene* scene, const Events::OnEngineStartup& event)
{
	screen_width = event.fb_width;
	screen_height = event.fb_height;

	screen_effective_width = screen_width;
	screen_effective_height = screen_height;
}

void InputHandler::mouseMoveScreen(FCS::Scene* scene, const Events::OnMouseMovement& event, bool active)
{
	glm::vec2 call_pos = glm::vec2(event.x, event.y);

	if (active)
	{
		auto main_cam = scene->getAllWith<Camera>()[0];
		auto cam_component = main_cam->getComponent<Camera>();

		// Calculate mouse translate speed on world position
		float translate_speed_x = math::map<float>(1, 0, screen_width, 0, screen_effective_width);
		float translate_speed_y = math::map<float>(1, 0, screen_height, 0, screen_effective_height);

		// Calculate diference
		global_translate += (global_last_translate - call_pos) * glm::vec2(translate_speed_x, translate_speed_y);
		cam_component->view(glm::vec2(global_translate.x, -global_translate.y));
	}

	global_last_translate = call_pos;

	// Keep mouse inside screen while dragging
	if (active)
	{
		bool over_x = call_pos.x > (float)screen_width;
		bool over_y = call_pos.y > (float)screen_height;
		bool under_x = call_pos.x < 0.0f;
		bool under_y = call_pos.y < 0.0f;

		bool mchanged = over_x || over_y || under_x || under_y;

		if (mchanged)
		{
			global_last_translate.x = over_x ? 0.0f : under_x ? (float)screen_width : global_last_translate.x;
			global_last_translate.y = over_y ? 0.0f : under_y ? (float)screen_height : global_last_translate.y;

			glfwSetCursorPos(event.window, global_last_translate.x, global_last_translate.y);
		}
	}
}
