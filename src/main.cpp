#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


#include "render/GLWrapper.h"
#include "assets/Registry.h"

#define DEBUG
#include "util/perf_counter.h"

//TEST
//#include "util/Serializer.h"


//TODO: Check and fix (duh!) all 'FIX' tags
//TODO: Run check tests with nsight to eliminate useless openGL API calls
//TODO: Makes FCS::Systems threadable
int main(int argc, char* argv[])
{
	GLOBAL_DEBUG_BLOCK_INIT;

	Application app;

	Registry registry_guard;

	/*Playing pstate;

	CSim csim;
	PSim psim;
	std::thread ct(&CSim::calculateStates, &csim);
	std::thread pt(&PSim::calculateStates, &psim);*/

	app.run();


	/*ct.join();
	pt.join();*/

	return 0;
}