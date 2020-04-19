#pragma once
#include "../core/ECS.h"
#include <GLFW/glfw3.h>

namespace Events
{
	struct OnMouseMovement
	{
		double x;
		double y;

		GLFWwindow* window;
	};

	struct OnMouseClick
	{
		int button;
		int action;
		int mods;

		GLFWwindow* window;
	};

	struct OnMouseScroll
	{
		double x;
		double y;

		GLFWwindow* window;
	};

	struct OnFramebufferResize
	{
		int width;
		int height;

		GLFWwindow* window;
	};

	struct OnEngineStartup
	{
		int fb_width;
		int fb_height;

		GLFWwindow* window;
	};
}

