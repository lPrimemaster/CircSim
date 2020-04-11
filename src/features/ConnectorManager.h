#pragma once
#include "Chunk.h"

class ConnectorManager
{
public:
	static std::pair<std::vector<Node*>, std::vector<Node*>> updateConnectorNode(Connector* c);
	static std::vector<Node*> updateInteractConnectorNode(InteractConnector* c);

private:
	ConnectorManager() = delete;
};

