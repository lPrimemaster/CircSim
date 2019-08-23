#pragma once
#include <utility>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "../../states/State.h"
#include <GLFW/glfw3.h>

#include "../../../deps/imgui/imgui.h"
#include "../../../deps/imgui/imgui_impl_glfw.h"
#include "../../../deps/imgui/imgui_impl_opengl3.h"


class Gui
{
private:
	Gui(GLFWwindow* window);
	~Gui();

public:
	inline static void setContext(GLFWwindow* window)
	{
		Gui::window = window;
	}

	inline static Gui& Get()
	{
		static Gui instance = Gui(window);
		return instance;
	}

	void newFrame() const;
	void render(GLFWwindow* window, State* state) const;

	void pushWindow(std::function<void(bool*, GLFWwindow*, State*)> body);
	void popWindow(unsigned id);
	bool* getWindowState(unsigned id);

private:
	static GLFWwindow* window;
	std::unordered_map<unsigned, std::pair<std::function<void(bool*, GLFWwindow*, State*)>, bool*>> window_hash;
};
