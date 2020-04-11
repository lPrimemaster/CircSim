#include "CSim.h"
#include <chrono>
#include <thread>

CSim::CSim()
{
}

CSim::~CSim()
{
}

void CSim::calculateStates()
{
	while (true)
	{
		for (auto reg_node : systemNodes)
		{
			calculateNode(reg_node.second);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

/* This is here so the nodes get "unticked" every tick so that the calculation goes on */
void CSim::registerNode(Node* node)
{
	systemNodes.insert(std::pair<unsigned, Node*>(reinterpret_cast<uintptr_t>(node), node));
}

void CSim::unregisterNode(Node* node)
{
	systemNodes.erase(reinterpret_cast<uintptr_t>(node));
	delete node;
}

std::unordered_map<uintptr_t, Node*> CSim::getRegisteredNodes() const
{
	return systemNodes;
}

/* Should be somewhat interesting to make this thread safe for later implementation, or even multithreaded recursion ??? is that a thing */
void CSim::calculateNode(Node* node)
{
	static int i = 0;
	auto deps = node->getDependencyNodes();
	bool any_dep_state = LOW;

	if (!deps.empty())
	{
		for (auto d : deps)
		{
			if (!d->getState())
			{
				any_dep_state = HIGH;
				break;
			}
		}

		node->write(any_dep_state);
	}
}
