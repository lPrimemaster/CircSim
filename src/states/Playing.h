#pragma once
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/ECS.h"
#include "../render/SpriteRenderer.h"
#include "../render/LineRenderer.h"

#include "../events/InputHandler.h"

#include "../util/math.h"

//TODO: Emplace the global callbacks inside the state as virtual functions to enable easier controll
class Playing : public FCS::Scene
{
public:
	friend void debug(bool* close, GLFWwindow* window);

	/* Overrides */
	void initialize() override;
	void deinitialize() override;

	/* Class specific code */

	/*const glm::mat4 getPVMatrix() const;
	const glm::mat4 getIPVMatrix() const;
	const glm::vec2 getMousePositionWorldSpace() const;

	static const glm::vec2 getTranslation();
	static const float getZScaling();*/

private:
	//glm::mat4 pview_mat;
	//glm::mat4 ipview_mat;

	//static float z_scl; //FIX: No need for static vars here
	//static float lx; //FIX: No need for static vars here
	//static float ly; //FIX: No need for static vars here

	//glm::vec2 frame_mouse_pos;

	//double last_cursor_x = 0.0;
	//double last_cursor_y = 0.0;

	//unsigned card;

	//bool update_next;

	////TODO: This shouldn't exist here! -> Each Chunk should manage its own gates
	//std::vector<Gate*> gate_tracker;

	//std::vector<Node*> require_update;
	//std::vector<Node*> require_unreg;

	//Application* debug_gw = nullptr;

	//struct LGStruct
	//{
	//	NotGate* not_gate = nullptr;
	//	SwitchGate* switch_gate = nullptr;

	//	glm::vec2 last_pos = glm::vec2(0.0f);
	//	bool drop = true;
	//}last_gates;

	//struct MousePick
	//{
	//	Gate* selected_gate = nullptr;
	//	bool isMovable = false;
	//}mouse_pick;
};
