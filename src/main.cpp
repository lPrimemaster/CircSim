#include <iostream>

#include "CSim.h"
#include "PSim.h"

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

	Registry registry_guard;

	Playing pstate;

	CSim csim;
	PSim psim;

	std::thread ct(&CSim::calculateStates, &csim);
	std::thread pt(&PSim::calculateStates, &psim);

	wrapper.run(&pstate, &csim, &psim);

	ct.join();
	pt.join();

	return 0;
}