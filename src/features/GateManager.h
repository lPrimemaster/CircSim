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

	template<typename T>
	static void createGate(std::vector<Chunk*> in_chunks, glm::vec2 initial_pos, glm::vec2 release_pos, T* gate, std::function<void(T*)> creator);

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

template<typename T>
inline void GateManager::createGate(std::vector<Chunk*> in_chunks, glm::vec2 initial_pos, glm::vec2 release_pos, T* gate, std::function<void(T*)> creator)
{
	GateManager::addGate<T>(gate);
	gate->isFixed() = true;
	gate->changeAlpha(1.0f);
	gate->update(initial_pos, release_pos);

	for(auto c : in_chunks)
		c->insertGate(gate);

	creator(gate);
}
