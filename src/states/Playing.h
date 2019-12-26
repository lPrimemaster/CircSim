#pragma once
#include <iostream>
#include "State.h"
#include "../render/GateRenderer.h"
#include "../render/GridRenderer.h"
#include "../gates/NGateDef.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../util/math.h"

//Test purposes only
#include "../features/ChunkManager.h"

#include "../render/gui/Gui.h"

#include "../Registry.h"

#include "../util/InputManager.h"

//TODO: Emplace the global callbacks inside the state as virtual functions to enable easier controll
class Playing : public State
{
public:
	friend void debug(bool* close, GLFWwindow* window, State* state);

	/* Overrides */

	void draw(GLWrapper* gw) override;
	void handle(GLWrapper* gw) override;
	void update(GLWrapper* gw) override;

	void scroll_callback(GLFWwindow* window, double xoff, double yoff) override;
	void click_callback(GLFWwindow* window, int button, int action, int mods) override;
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	void initialize(GLWrapper* gw) override;
	void cleanUp() override;

	/* Class specific code */

	const glm::mat4 getPVMatrix() const;
	const glm::mat4 getIPVMatrix() const;
	const glm::vec2 getMousePositionWorldSpace() const;

	//TODO: This shouldn't exist here! -> Each Chunk should manage its own gates
	void createGate(glm::vec2 in, glm::vec2 out);

	static const glm::vec2 getTranslation();
	static const float getZScaling();

private:
	GateRenderer gate_renderer;
	GridRenderer grid_renderer;

	glm::mat4 pview_mat;
	glm::mat4 ipview_mat;

	static float z_scl; //FIX: No need for static vars here
	static double lx; //FIX: No need for static vars here
	static double ly; //FIX: No need for static vars here

	glm::vec2 frame_mouse_pos;

	double last_cursor_x = 0.0;
	double last_cursor_y = 0.0;

	unsigned card;

	Gate* gate[10] = { nullptr }; //Test gates

	//TODO: This shouldn't exist here! -> Each Chunk should manage its own gates
	std::vector<Gate*> gate_tracker;
};
