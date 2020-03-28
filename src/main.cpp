#include <iostream>

#include "CSim.h"
#include "PSim.h"

#include "render/GLWrapper.h"
#include "states/Playing.h"

#include "util/memtrace.h"

#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <fstream>

//TEST
//#include "util/Serializer.h"

//TODO: Check and fix (duh!) all 'FIX' tags
//TODO: Run check tests with nsight to eliminate useless openGL API calls
int main(int argc, char* argv[])
{
	//TEST
	/*serializer::OutArchive out_ar;
	std::vector<int> vo(100, 1);
	out_ar.allocate(sizeof(int) * 102);
	out_ar.process(3);
	out_ar.process<std::vector<int>, int>(vo);

	out_ar.save("test_save.bin");

	serializer::InArchive in_ar;
	in_ar.load("test_save.bin");
	int x;
	std::vector<int> vi;
	in_ar.process(&x);
	in_ar.process<std::vector<int>, int>(&vi);
	std::cout << x << std::endl;*/

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