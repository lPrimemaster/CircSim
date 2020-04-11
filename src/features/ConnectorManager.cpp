#include "ConnectorManager.h"
#include "ChunkManager.h"

std::vector<Node*> ConnectorManager::updateConnectorNode(Connector* c)
{
	//Check where is this connector at
	Chunk* chunk = ChunkManager::getChunkAtPosition(c->position);

	std::vector<Node*> need_registry;
	bool isOnly = false;

	//FIX: This can be a problem when moving various coupled connectors at the same time
	if (c->node != nullptr)
	{
		if (c->node->getConnectorsNum() != 0)
		{
			c->node->dec();
			c->node = nullptr;
		}
		else
		{
			isOnly = true;
		}
	}

	//Check if there is any node already in the connector position
	for (auto con : chunk->connector_list)
	{
		if (con == c) continue; //Ignore self

		if (con->node != nullptr && con->isOverlapped(*c))
		{
			if (isOnly)
			{
				std::cout << "----------\n";
				std::cout << "Delete Node" << "\nID == " << c->node->getName() << "\nType == " << typeid(Connector).name();
				std::cout << "\n----------\n";

				delete c->node;
			}

			c->node = con->node;
			c->node->inc();
			break;
		}
	}

	//Then no node was found in this chunk that could be related to this connector
	if (c->node == nullptr && !isOnly)
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
		isOnly = false;
		//FIX: This can be a problem when moving various coupled connectors at the same time
		if (cd->node != nullptr)
		{
			if (cd->node->getConnectorsNum() != 0)
			{
				cd->node->dec();
				cd->node = nullptr;
			}
			else
			{
				isOnly = true;
			}
		}

		Chunk* dep_chunk = ChunkManager::getChunkAtPosition(cd->position);
		for (auto con : dep_chunk->connector_list)
		{
			if (con == cd) continue; //Ignore self

			if (con->node != nullptr && con->isOverlapped(*cd))
			{
				if (isOnly)
				{
					std::cout << "----------\n";
					std::cout << "Delete Node" << "\nID == " << c->node->getName() << "\nType == " << typeid(Connector).name();
					std::cout << "\n----------\n";

					//FIX: This should delete the old connector (c) dependencies node, not the recentrly created one ...
					delete cd->node;
				}

				cd->node = con->node;
				cd->node->inc();
				break;
			}
		}

		if (cd->node == nullptr && !isOnly)
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
			//FIX
			c->node->setDependencyNode(cd->node);
		}
	}
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