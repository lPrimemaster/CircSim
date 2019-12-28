#include "CSim.h"
#include <chrono>
#include <thread>

CSim::CSim()
{
}

CSim::~CSim()
{
}

void CSim::analyzeConnections()
{
	for (auto out : outputs)
	{
		auto node = out;
		while (true)
		{
			for (auto nodes : node->getDependencyNodes())
			{
				//WTH is this even here for
			}
		}
	}
}

void CSim::calculateStates()
{
	while (true)
	{
		for (auto reg_node : systemNodes)
		{
			calculateNode(reg_node.second);
		}

		for(auto reg_node : systemNodes)
		{
			printf("Node %s -> %s\n", reg_node.second->getName().c_str(), reg_node.second->getState() ? "HIGH" : "LOW ");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Limit to 500 TPS

#ifdef _WIN32 //This is choppy but it works
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD topLeft = { 0, 0 };

		std::cout.flush();

		if (!GetConsoleScreenBufferInfo(hOut, &csbi))
			abort();

		DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

		DWORD written;
		FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

		FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

		SetConsoleCursorPosition(hOut, topLeft);
#else
		printf("\033[2J\033[1;1H"); //Clear screen
#endif
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
