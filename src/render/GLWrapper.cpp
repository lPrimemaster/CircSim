#include "GLWrapper.h"
#include "../states/State.h"

#define ERROR_INIT -1
#define ERROR_WINDOW -2


GLWrapper::GLWrapper()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW failed to initialize." << std::endl;
		exit(ERROR_INIT);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(1280, 720, "CircSim Dev", NULL, NULL);

	if (!window)
	{
		std::cerr << "GLFW failed to create window." << std::endl;
		exit(ERROR_WINDOW);
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0); //No cap for test performance

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


GLWrapper::~GLWrapper()
{
	glfwTerminate();
}

GLFWwindow* GLWrapper::getWindow() const
{
	return window;
}

CSim* GLWrapper::getSimulation() const
{
	return simulation;
}

glm::vec2 GLWrapper::getWindowDim() const
{
	return glm::vec2(windowW, windowH);
}

void GLWrapper::draw(State* state)
{
	glClear(GL_COLOR_BUFFER_BIT);
	state->draw(this);
	glfwSwapBuffers(window);
}

void GLWrapper::handleEvents(State* state)
{
	glfwPollEvents();
	state->handle(this);
}

void GLWrapper::update(State* state)
{
	state->update(this);
}

void GLWrapper::run(State* state, CSim* simulation)
{
	//This needs to be moved / refactored to a diferend init scheme
	state->initialize(this);
	this->simulation = simulation;

	while (!glfwWindowShouldClose(window))
	{
		//Handle window reajusts
		int w, h;
		//Reduce this overheading with call of this on readjust
		glfwGetFramebufferSize(window, &w, &h);
		windowW = w;
		windowH = h;
		float ratio = (float) w / h;
		glViewport(0, 0, w, h);

		//Run the main program
		handleEvents(state);
		update(state);
		draw(state);
	}

	state->cleanUp();

	glfwDestroyWindow(window);
}
