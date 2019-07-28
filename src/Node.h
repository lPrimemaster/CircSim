#pragma once
#include <iostream>
#include <vector>

#define LOW  0
#define HIGH 1

class Gate;

/* The nodes must be evaluated in a specifically determined hierarchical manner to avoid a re-evaluation requirement */

class Node
{
public:
	Node();
	~Node();

	bool getTickedState() const;
	bool getState() const;

	void tick();
	void untick();

	void setDependencyNode(Node* node);
	std::vector<Node*> getDependencyNodes() const;

	void write(bool input);

private:
	std::vector<Node*> dependencies;

private:
	bool state = LOW;
	bool evaluated = false;
};

