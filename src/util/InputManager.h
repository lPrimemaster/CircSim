#pragma once
#include "../states/State.h"
#include "../states/Playing.h"

typedef enum { BASE, PLAYING } DState;

struct StateInfo
{
	State* state = nullptr;
	DState derived_state = BASE;
};

class InputManager
{
public:
	static void setActiveWindow(GLFWwindow* window);
	static void setCallbackControllerState(StateInfo info);

private:
	static void scroll_callback(GLFWwindow* window, double xoff, double yoff);
	static void click_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void move_callback(GLFWwindow* window, double xpos, double ypos);

private:
	InputManager() = delete;
	~InputManager() = delete;

private:
	static GLFWwindow* window;
	static StateInfo info;
};
