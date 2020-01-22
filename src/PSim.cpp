#include "PSim.h"
#include <chrono>

PSim::PSim()
{
}

PSim::~PSim()
{
}

void PSim::calculateStates()
{
	calculateMouseIntersection();
}

void PSim::setMouseAtomic(const glm::vec2& mouse)
{
	this->mouse_x.store(mouse.x);
	this->mouse_y.store(mouse.y);
}

void PSim::setCurrentActiveChunkAtomic(const Chunk* chunk)
{
	current_active.store(chunk);
}

inline void PSim::calculateIntersections()
{
}

inline void PSim::calculateMouseIntersection()
{
	unsigned int count = 0;
	while (true)
	{
		if (const Chunk* ca = current_active.load(); ca != nullptr)
		{
			auto gates = ca->getGateList();
			for (auto gate : gates)
			{
				auto bound = gate->getBoundings();
				if (bound.intersect(glm::vec2(mouse_x.load(), mouse_y.load())))
				{
					std::cout << "Mouse intersect with gate! [" << count++ << "]" << std::endl;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
