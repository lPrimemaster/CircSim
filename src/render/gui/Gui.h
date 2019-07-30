#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>

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
	void render() const;

	void pushWindow(std::function<bool*> body);
	void popWindow(unsigned id);

private:
	static GLFWwindow* window;
	std::unordered_map<unsigned, std::pair<std::function<void(bool*)>, bool*>> window_hash;
};
