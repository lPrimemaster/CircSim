#include "Node.h"

Node::Node()
{
}

Node::~Node()
{
	dependencies.clear();
}

bool Node::getTickedState() const
{
	return evaluated;
}

bool Node::getState() const
{
	return state;
}

void Node::tick()
{
	evaluated = true;
}

void Node::untick()
{
	evaluated = false;
}

void Node::setDependencyNode(Node* node)
{
	dependencies.push_back(node);
}

std::vector<Node*> Node::getDependencyNodes() const
{
	return dependencies;
}

void Node::write(bool input)
{
	state = input;
}
