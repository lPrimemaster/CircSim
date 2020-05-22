#pragma once
#include <iostream>
#include "../render/Program.h"
#include "../core/ECS.h"

class Application
{
public:
	Application();
	~Application();

public:
	void run();

private:
	GLFWwindow* window = nullptr;

private:
	int windowW = 0;
	int windowH = 0;
};

