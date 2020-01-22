#include "ConnectorManager.h"
#include "ChunkManager.h"

std::vector<Node*> ConnectorManager::updateConnectorNode(Connector* c)
{
	//Check where is this connector at
	Chunk* chunk = ChunkManager::getChunkAtPosition(c->position);

	std::vector<Node*> need_registry;
	//Check if there is any node already in the connector position

	for (auto con : chunk->connector_list)
	{
		if (con->node != nullptr && con->isOverlapped(*c))
			c->node = con->node;
	}

	//Then no node was found in this chunk that could be related to this connector
	if (c->node == nullptr)
	{
		//TODO: Change this to a hex value naming system
		c->node = new Node(math::generateHEX());

		std::cout << "----------\n";
		std::cout << "New Node" << "\nID == " << c->node->getName() << "\nType == " << typeid(Connector).name();
		std::cout << "\n----------\n";

		need_registry.push_back(c->node);
	}

	//Check connector dependencies and do the same
	for (auto cd : c->dep_conector)
	{
		Chunk* dep_chunk = ChunkManager::getChunkAtPosition(cd->position);
		for (auto con : dep_chunk->connector_list)
		{
			if (con->node != nullptr && con->isOverlapped(*cd))
				cd->node = con->node;
		}

		if (cd->node == nullptr)
		{
			//TODO: Change this to a hex value naming system
			cd->node = new Node(math::generateHEX());

			std::cout << "----------\n";
			std::cout << "New Node" << "\nID == " << cd->node->getName() << "\nType == " << typeid(Connector).name();
			std::cout << "\n----------\n";

			need_registry.push_back(cd->node);
		}

		if (cd->node)
		{
			c->node->setDependencyNode(cd->node);
		}
	}
	std::cout << "\n\n";
	return need_registry;
}

std::vector<Node*> ConnectorManager::updateInteractConnectorNode(InteractConnector* c)
{
	//Check where is this connector at
	Chunk* chunk = ChunkManager::getChunkAtPosition(c->position);

	std::vector<Node*> need_registry;

	//Then no node was found in this chunk that could be related to this connector
	if (c->node == nullptr)
	{
		//TODO: Change this to a hex value naming system
		c->node = c->output_interact->node;

		std::cout << "----------\n";
		std::cout << "New Node" << "\nID == " << c->node->getName() << "\nType == " << typeid(InteractConnector).name();
		std::cout << "\n----------\n";

		need_registry.push_back(c->node);
	}
	//For now interactor connectors don't have dependencies
	std::cout << "\n\n";
	return need_registry;
}