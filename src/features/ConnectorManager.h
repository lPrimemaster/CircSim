#pragma once
#include "Chunk.h"

class ConnectorManager
{
public:
	static std::vector<Node*> updateConnectorNode(Connector* c);
	static std::vector<Node*> updateInteractConnectorNode(InteractConnector* c);

private:
	ConnectorManager() = delete;
};

