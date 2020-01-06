#include <iostream>

#include "CSim.h"
#include "render/GLWrapper.h"
#include "states/Playing.h"

#include "util/memtrace.h"

#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <fstream>

//TODO: Check and fix (duh!) all 'FIX' tags
//TODO: Run check tests with nsight to eliminate useless openGL API calls
int main(int argc, char* argv[])
{
	//Todo : Use main thread for drawing and create as necessary to the circuit evaluation
	GLWrapper wrapper;

	Registry::registerAllBasicGeometry();
	Registry::registerAllCharAtlas();

	Playing pstate;

	CSim csim;
	std::thread nt(&CSim::calculateStates, &csim);

	wrapper.run(&pstate, &csim);

	nt.join();

	Registry::unregisterAllCharAtlas();
	Registry::unregisterAllBasicGeometry();

	return 0;
}