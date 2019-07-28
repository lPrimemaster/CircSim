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
	// Test only
	//And gate
	Node a, b, c, d;

	a.write(HIGH);
	b.write(HIGH);

	c.setDependencyNode(&a);
	c.setDependencyNode(&b);
	d.setDependencyNode(&c);

	calculateNode(&d);

	printf("Node A -> %s\n", a.getState() ? "HIGH" : "LOW");
	printf("Node B -> %s\n", b.getState() ? "HIGH" : "LOW");
	printf("Node C -> %s\n", c.getState() ? "HIGH" : "LOW");
	printf("Node D -> %s\n", d.getState() ? "HIGH" : "LOW");

	//RS Flip-flop / Latch test
	//Attempt a progressive method for timers? -- NOTICE THIS

	Node N0, N1, N2, N3, N4, N5, N6, N7, N8, N9;


	N2.setDependencyNode(&N0);
	N3.setDependencyNode(&N1);
	N4.setDependencyNode(&N2);
	N4.setDependencyNode(&N8);
	N5.setDependencyNode(&N3);
	N5.setDependencyNode(&N9);

	N6.setDependencyNode(&N4);
	N7.setDependencyNode(&N5);

	N8.setDependencyNode(&N7);
	N9.setDependencyNode(&N6);

	N0.write(HIGH); //R
	N1.write(LOW);  //S
	N6.write(HIGH); //Q
	calculateNode(&N6);

	auto clear = [&]() -> void
	{
		N0.untick();
		N1.untick();
		N2.untick();
		N3.untick();
		N4.untick();
		N5.untick();
		N6.untick();
		N7.untick();
		N8.untick();
		N9.untick();
	};
	auto print = [&]() -> void
	{
		printf("N0 -> %s [R]\n", N0.getState() ? "HIGH" : "LOW");
		printf("N1 -> %s [S]\n", N1.getState() ? "HIGH" : "LOW");
		printf("N2 -> %s\n", N2.getState() ? "HIGH" : "LOW");
		printf("N3 -> %s\n", N3.getState() ? "HIGH" : "LOW");
		printf("N4 -> %s\n", N4.getState() ? "HIGH" : "LOW");
		printf("N5 -> %s\n", N5.getState() ? "HIGH" : "LOW");
		printf("N6 -> %s [Q+]\n", N6.getState() ? "HIGH" : "LOW");
		printf("N7 -> %s [Q-]\n", N7.getState() ? "HIGH" : "LOW");
		printf("N8 -> %s\n", N8.getState() ? "HIGH" : "LOW");
		printf("N9 -> %s\n", N9.getState() ? "HIGH" : "LOW");
	};
	
	clear();

	N0.write(LOW); //R
	N1.write(LOW); //S
	calculateNode(&N6);

	print();
	

	clear();

	N0.write(LOW);
	N1.write(HIGH);
	calculateNode(&N6);

	print();

	registerNode(&N6);

	int i = 0;

	while (true)
	{
		i++;
		//Clear all registered nodes tick status
		for (auto reg_node : systemNodes)
		{
			clearAll(reg_node.second);
		}

		for (auto reg_node : systemNodes)
		{
			calculateNode(reg_node.second);
		}

		for (auto reg_node : systemNodes)
		{
			printf("Node %p is %s\r", (void*)reg_node.first, reg_node.second->getState() ? "HIGH" : "LOW ");
		}

		if (!(i % 10)) { N0.write(HIGH); N1.write(LOW); }
		if (!(i % 11)) { N0.write(LOW); N1.write(LOW); }
		if (!(i % 20)) { N0.write(LOW); N1.write(HIGH); }
		if (!(i % 21)) { N0.write(LOW); N1.write(LOW); }

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		if (i > 300) break;
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
	node->tick();

	if (!deps.empty())
	{
		bool any_dep_state = LOW;
		for (auto d : deps)
		{
			if (!d->getTickedState())
			{
				calculateNode(d);
			}
			if (!d->getState())
				any_dep_state = HIGH;
		}
		/*if (!node->getState() && any_dep_state)
			printf("[It %d] State of node %p switched to HIGH.\n", i++, (void*)node);
		else if (!node->getState() && !any_dep_state)
			printf("[It %d] State of node %p kept at LOW.\n", i++, (void*)node);
		else if(node->getState() && any_dep_state)
			printf("[It %d] State of node %p kept at HIGH.\n", i++, (void*)node);
		else
			printf("[It %d] State of node %p switched to LOW.\n", i++, (void*)node);*/
		node->write(any_dep_state);
	}
}

void CSim::clearAll(Node* node)
{
	auto deps = node->getDependencyNodes();
	node->untick();

	if (!deps.empty())
	{
		for (auto d : deps)
		{
			if (d->getTickedState())
			{
				clearAll(d);
			}
		}
	}
}
