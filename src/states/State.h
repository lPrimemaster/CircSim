#pragma once
#include "../render/GLWrapper.h"

class State
{
public:
	virtual ~State() {  }

	virtual void draw(GLWrapper * gw) = 0;
	virtual void handle(GLWrapper * gw) = 0;
	virtual void update(GLWrapper * gw) = 0;

	virtual void scroll_callback(GLFWwindow* window, double xoff, double yoff) = 0;
	virtual void click_callback(GLFWwindow* window, int button, int action, int mods) = 0;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	virtual void move_callback(GLFWwindow* window, double xpos, double ypos) = 0;

	virtual void initialize(GLWrapper * gw) = 0;
	virtual void cleanUp() = 0;
};

