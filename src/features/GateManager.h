#pragma once
#include <vector>
#include "../gates/Gate.h"
#include "ChunkManager.h"

class GateManager
{
public:
	template<typename T>
	static void addGate(T* gate);

	template<typename T>
	static void deleteGate(T* gate);

private:
	static std::vector<Gate*> global_gate_list;

private:
	GateManager() = delete;
};

template<typename T>
inline void GateManager::addGate(T* gate)
{
	global_gate_list.push_back(gate);
	
	auto& in_ports = gate->getInputs();
	auto& out_ports = gate->getOutputs();

	//Check if the gate is inside one, two, or more chunks
	std::vector<ChunkCoord> c_index;
	for (auto& in : in_ports)
	{
		ChunkCoord cc = ChunkManager::getChunkIndexAtPosition(in->position);
		if (std::find(c_index.cbegin(), c_index.cend(), cc) == c_index.cend())
		{
			ChunkManager::createChunkAtPosition(cc);
			ChunkManager::getChunkAtPosition(cc)->insertConnector(in);
			c_index.push_back(cc);
		}
	}

	c_index.clear();

	for (auto& out : out_ports)
	{
		ChunkCoord cc = ChunkManager::getChunkIndexAtPosition(out->position);
		if (std::find(c_index.cbegin(), c_index.cend(), cc) == c_index.cend())
		{
			ChunkManager::createChunkAtPosition(cc);
			ChunkManager::getChunkAtPosition(cc)->insertConnector(out);
			c_index.push_back(cc);
		}
	}
}
