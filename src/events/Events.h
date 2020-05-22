#pragma once
#include "../core/ECS.h"

struct GLFWwindow;

namespace Events
{
	template<typename T>
	struct OnMouseMovement
	{
		double x;
		double y;

		GLFWwindow* window;
	};

	namespace MovementType
	{
		struct ScreenSpace;
		struct WorldSpace;
	}

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

	struct OnKeyPressed
	{
		int key;
		int action;
		int mods;

		GLFWwindow* window;
	};

	struct OnPCounterInitDebugInfo
	{

	};
}

