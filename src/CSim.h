#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Node.h"

#include "util/memtrace.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class CSim
{
public:
	CSim();
	~CSim();

	void calculateStates();
	void registerNode(Node* node); //If a node is not registered the system will not evaluate it
	void unregisterNode(Node* node);

	std::unordered_map<uintptr_t, Node*> getRegisteredNodes() const;

private:
	inline void calculateNode(Node* node);

private:
	//List to get track off all the existing nodes in the system
	std::unordered_map<uintptr_t, Node*> systemNodes;
};

