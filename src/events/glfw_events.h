#pragma once
#include "../render/GLWrapper.h"

void glfw_cursorpos_callback(GLFWwindow* window, double x, double y);
void glfw_mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
void glfw_mousescroll_callback(GLFWwindow* window, double x, double y);
void glfw_framebufferresize_callback(GLFWwindow* window, int w, int h);
