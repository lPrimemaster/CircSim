#pragma once
#include "features/Chunk.h"
#include "gates/Gate.h"

class PSim
{
public:
	PSim();
	~PSim();

	void calculateStates();
	void setMouseAtomic(const glm::vec2& mouse);
	void setCurrentActiveChunkAtomic(const Chunk* chunk);

private:
	inline void calculateIntersections();
	inline void calculateMouseIntersection();

private:
	float tick_rate = 10.0f;

private:
	std::atomic<const Chunk*> current_active = nullptr;
	Gate* current_selected = nullptr;
	std::atomic<float> mouse_x;
	std::atomic<float> mouse_y;
};

