#include "InputManager.h"

#define CREATE_STATE(type, etype, callback, ...) \
case etype:\
{\
type* state = static_cast<type*>(info->state);\
state->callback(##__VA_ARGS__);\
}\
break;

GLFWwindow* InputManager::window = nullptr;
StateInfo InputManager::info;

void InputManager::setActiveWindow(GLFWwindow* window)
{
	InputManager::window = window;
	glfwSetScrollCallback(window, InputManager::scroll_callback);
	glfwSetMouseButtonCallback(window, InputManager::click_callback);
	glfwSetKeyCallback(window, InputManager::key_callback);
}

void InputManager::setCallbackControllerState(StateInfo info)
{
	InputManager::info = info;

	if (window != nullptr)
	{
		glfwSetWindowUserPointer(window, &InputManager::info);
	}
	else
	{
		//TODO: Handle error
	}
}

void InputManager::scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
	//TODO: Is it okay to get it every time - on every single callback?
	StateInfo* info = static_cast<StateInfo*>(glfwGetWindowUserPointer(window));

	switch (info->derived_state)
	{
	case BASE: break; //TODO: Base class shouldn't be called what so ever (handle error)

	CREATE_STATE(Playing, PLAYING, scroll_callback, window, xoff, yoff);

	default: break;
	}
}

void InputManager::click_callback(GLFWwindow* window, int button, int action, int mods)
{
	//TODO: Is it okay to get it every time - on every single callback?
	StateInfo* info = static_cast<StateInfo*>(glfwGetWindowUserPointer(window));

	switch (info->derived_state)
	{
	case BASE: break; //TODO: Base class shouldn't be called what so ever (handle error)

	CREATE_STATE(Playing, PLAYING, click_callback, window, button, action, mods);

	default: break;
	}
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//TODO: Is it okay to get it every time - on every single callback?
	StateInfo* info = static_cast<StateInfo*>(glfwGetWindowUserPointer(window));

	switch (info->derived_state)
	{
	case BASE: break; //TODO: Base class shouldn't be called what so ever (handle error)

		CREATE_STATE(Playing, PLAYING, key_callback, window, key, scancode, action, mods);

	default: break;
	}
}
