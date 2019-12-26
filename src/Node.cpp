#include "Node.h"
#include "gates/NGateDef.h"

Node::Node(const std::string name)
{
	this->name = name;
}

Node::~Node()
{
	dependencies.clear();
}

bool Node::getState() const
{
	return state;
}

std::string Node::getName() const
{
	return name;
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
