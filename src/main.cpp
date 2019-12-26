#include <iostream>

#include "CSim.h"
#include "render/GLWrapper.h"
#include "states/Playing.h"

#include "util/memtrace.h"

#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <fstream>

int main(int argc, char* argv[])
{
	//Todo : Use main thread for drawing and create as necessary to the circuit evaluation
	GLWrapper wrapper;

	Registry::registerAllBasicGeometry();

	Playing pstate;

	CSim csim;
	std::thread nt(&CSim::calculateStates, &csim);

	wrapper.run(&pstate, &csim);

	nt.join();

	Registry::unregisterAllBasicGeometry();

	return 0;
}