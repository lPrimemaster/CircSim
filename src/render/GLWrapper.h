#pragma once
#include <iostream>
#include "Program.h"
#include "../CSim.h"
#include "../PSim.h"

class State;

class GLWrapper
{
public:
	GLWrapper();
	~GLWrapper();

	GLFWwindow* getWindow() const;
	CSim* getSimulation() const;
	PSim* getPhysics() const;

	glm::vec2 getWindowDim() const;

	/* This stuff can be replaced in the future for an event driven type of class (State manager like) */
	/* Using stuff like this is not typical of me */
protected:
	void draw(State * state);
	void handleEvents(State * state);
	void update(State * state);

public:
	void run(State * state, CSim * simulation, PSim* physics);

private:
	GLFWwindow* window = nullptr;
	CSim* simulation = nullptr;
	PSim* physics = nullptr;

private:
	int windowW = 0;
	int windowH = 0;
};

