#pragma once
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

private:
	static GLFWwindow* window;
};
