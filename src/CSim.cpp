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
	Node S0("S0"), R0("R0"), S_0("S-0"), R_0("R-0"), Q0("Q0"), Q_0("Q-0");
	Node S1("S1"), R1("R1"), S_1("S-1"), R_1("R-1"), Q1("Q1"), Q_1("Q-1");
	Node S2("S2"), R2("R2"), S_2("S-2"), R_2("R-2"), Q2("Q2"), Q_2("Q-2");
	Node S3("S3"), R3("R3"), S_3("S-3"), R_3("R-3"), Q3("Q3"), Q_3("Q-3");

	// Uncomment for T-FlipFlop
	S_0.setDependencyNode(&Q_0);
	R_0.setDependencyNode(&Q0);

	R_0.setDependencyNode(&R0);
	S_0.setDependencyNode(&S0);

	Q0.setDependencyNode(&S_0);
	Q0.setDependencyNode(&Q_0);

	Q_0.setDependencyNode(&R_0);
	Q_0.setDependencyNode(&Q0);

	registerNode(&S0);
	registerNode(&R0);
	registerNode(&S_0);
	registerNode(&R_0);
	registerNode(&Q0);
	registerNode(&Q_0);

	// Uncomment for T-FlipFlop
	S1.setDependencyNode(&Q0);
	R1.setDependencyNode(&Q0);

	S_1.setDependencyNode(&Q_1);
	R_1.setDependencyNode(&Q1);

	S_1.setDependencyNode(&S1);
	R_1.setDependencyNode(&R1);

	Q1.setDependencyNode(&S_1);
	Q1.setDependencyNode(&Q_1);

	Q_1.setDependencyNode(&R_1);
	Q_1.setDependencyNode(&Q1);

	registerNode(&S1);
	registerNode(&R1);
	registerNode(&S_1);
	registerNode(&R_1);
	registerNode(&Q1);
	registerNode(&Q_1);

	// // Uncomment for T-FlipFlop
	// S2.setDependencyNode(&Q1);
	// R2.setDependencyNode(&Q1);
	
	// S_2.setDependencyNode(&Q_2);
	// R_2.setDependencyNode(&Q2);

	// S_2.setDependencyNode(&S2);
	// R_2.setDependencyNode(&R2);

	// Q2.setDependencyNode(&S_2);
	// Q2.setDependencyNode(&Q_2);

	// Q_2.setDependencyNode(&R_2);
	// Q_2.setDependencyNode(&Q2);

	// registerNode(&S2);
	// registerNode(&R2);
	// registerNode(&S_2);
	// registerNode(&R_2);
	// registerNode(&Q2);
	// registerNode(&Q_2);

	// // Uncomment for T-FlipFlop
	// S3.setDependencyNode(&Q2);
	// R3.setDependencyNode(&Q2);

	// S_3.setDependencyNode(&Q_3);
	// R_3.setDependencyNode(&Q3);

	// S_3.setDependencyNode(&S3);
	// R_3.setDependencyNode(&R3);

	// Q3.setDependencyNode(&S_3);
	// Q3.setDependencyNode(&Q_3);

	// Q_3.setDependencyNode(&R_3);
	// Q_3.setDependencyNode(&Q3);

	// registerNode(&S3);
	// registerNode(&R3);
	// registerNode(&S_3);
	// registerNode(&R_3);
	// registerNode(&Q3);
	// registerNode(&Q_3);

	auto printOut = [&]() -> void
	{
		printf("B0 -> %s\n", Q0.getState() ? "HIGH" : "LOW ");
		printf("B1 -> %s\n", Q1.getState() ? "HIGH" : "LOW ");
		printf("B2 -> %s\n", Q2.getState() ? "HIGH" : "LOW ");
		printf("B3 -> %s\n", Q3.getState() ? "HIGH" : "LOW ");
	};

	int i = 0;

	bool set = false;
	bool reset = false;
	bool l = false;

	while (true)
	{
		i++;

		for (auto reg_node : systemNodes)
		{
			calculateNode(reg_node.second);
		}

		for(auto reg_node : systemNodes)
		{
			printf("Node %s -> %s\n", reg_node.second->getName().c_str(), reg_node.second->getState() ? "HIGH" : "LOW ");
		}

		printOut();

		printf("Tick: %d\n", i);

		if(set)
		{
			printf("Set!\n");
		}
		else if(reset)
		{
			printf("Reset!\n");
		}
		else
		{
			printf("Waiting for set or reset...\n");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Limit to 10 TPS

	if(i == 50)
	{
		S0.write(HIGH);
		R0.write(HIGH);
	}
	else if(i == 70)
	{
		S0.write(LOW);
		R0.write(LOW);
	}
	else if(i == 90)
	{
		S0.write(HIGH);
		R0.write(HIGH);
	}

		// if(!set && !(i % 40) && !l) { S0.write(HIGH); R0.write(HIGH); set = true; reset = false; }
		// else if(set) { S0.write(LOW); R0.write(LOW); set = false; reset = true; l = true; }
		// else if(l) { S0.write(HIGH); R0.write(HIGH); set = true; l = false; }
		// if(set && !(i % 20)) { S0.write(LOW); R0.write(HIGH); set = false; reset = true; }
		// else if(reset) { S0.write(LOW); R0.write(LOW); }

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
