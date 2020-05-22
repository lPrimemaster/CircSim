#include "glfw_events.h"
#include "../core/ECS.h"
#include "Events.h"

void glfw_cursorpos_callback(GLFWwindow* window, double x, double y)
{
	Events::OnMouseMovement<Events::MovementType::ScreenSpace> mme;
	mme.x = x;
	mme.y = y;
	mme.window = window;

	FCS::SceneManager::GetActiveScene()->emit<Events::OnMouseMovement<Events::MovementType::ScreenSpace>>(mme);
}

void glfw_mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	Events::OnMouseClick mce;
	mce.button = button;
	mce.action = action;
	mce.mods = mods;
	mce.window = window;

	FCS::SceneManager::GetActiveScene()->emit<Events::OnMouseClick>(mce);
}

void glfw_mousescroll_callback(GLFWwindow* window, double x, double y)
{
	Events::OnMouseScroll mse;
	
	mse.x = x;
	mse.y = y;
	mse.window = window;

	FCS::SceneManager::GetActiveScene()->emit<Events::OnMouseScroll>(mse);
}

void glfw_framebufferresize_callback(GLFWwindow* window, int w, int h)
{
	Events::OnFramebufferResize fre;

	fre.width = w;
	fre.height = h;
	fre.window = window;

	glViewport(0, 0, w, h);

	FCS::SceneManager::GetActiveScene()->emit<Events::OnFramebufferResize>(fre);
}

void glfw_keypressed_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Events::OnKeyPressed kpe;

	kpe.key = key;
	kpe.action = action;
	kpe.mods = mods;
	kpe.window = window;

	FCS::SceneManager::GetActiveScene()->emit<Events::OnKeyPressed>(kpe);
}
