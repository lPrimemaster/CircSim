#include "GLWrapper.h"
#include "../core/ECS.h"
#include "../states/Playing.h"
#include "../events/glfw_events.h"

#include "../util/perf_counter.h"

#define ERROR_INIT -1
#define ERROR_WINDOW -2

#define DEBUG

#ifdef DEBUG
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(type == GL_DEBUG_TYPE_ERROR)
		fprintf(stderr, "GL ERROR: type = 0x%x, severity = 0x%x, message = %s\n\n", type, severity, message);
}
#endif


Application::Application()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW failed to initialize." << std::endl;
		exit(ERROR_INIT);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 4);

	windowW = 1280;
	windowH = 720;

	window = glfwCreateWindow(windowW, windowH, "CircSim Dev", NULL, NULL);

	if (!window)
	{
		std::cerr << "GLFW failed to create window." << std::endl;
		exit(ERROR_WINDOW);
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0); //No cap for performance test

	glewExperimental = true;
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize - Cause: " << glewGetErrorString(status) << std::endl;
	}
	else
	{
		std::cerr << "Glew initialized sucessfully - VERSION " << glewGetString(GLEW_VERSION) << std::endl;
	}

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set glfw callbacks
	glfwSetCursorPosCallback(window, glfw_cursorpos_callback);
	glfwSetMouseButtonCallback(window, glfw_mousebutton_callback);
	glfwSetScrollCallback(window, glfw_mousescroll_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebufferresize_callback);

	// During init, enable debug output
#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif
}


Application::~Application()
{
	glfwTerminate();
}

GLFWwindow* Application::getWindow() const
{
	return window;
}

glm::vec2 Application::getWindowDim() const
{
	return glm::vec2(windowW, windowH);
}

void Application::run()
{
	// TODO: This needs to be moved / refactored to a diferend init scheme
	FCS::SceneManager::LoadScene<Playing>();

	Events::OnEngineStartup ese;
	ese.fb_height = windowH;
	ese.fb_width = windowW;
	ese.window = window;
	FCS::SceneManager::GetActiveScene()->emit<Events::OnEngineStartup>(ese);

	while (!glfwWindowShouldClose(window))
	{
		full_record.cycles -= __rdtsc();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		FCS::SceneManager::Update();
		glfwSwapBuffers(window);
		ppPerfCounterRecords();

		full_record.cycles += __rdtsc();
	}

	FCS::SceneManager::UnloadAllScenes();

	glfwDestroyWindow(window);
}
