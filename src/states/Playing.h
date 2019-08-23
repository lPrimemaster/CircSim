#pragma once
#include <iostream>
#include "State.h"
#include "../render/GateRenderer.h"
#include "../render/GridRenderer.h"
#include "../gates/NGateDef.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../util/math.h"

#include "../render/gui/Gui.h"

#include "../Registry.h"

class Playing : public State
{
public:
	friend void scroll_callback(GLFWwindow* window, double xoff, double yoff);

	void draw(GLWrapper* gw) override;
	void handle(GLWrapper* gw) override;
	void update(GLWrapper* gw) override;

	void initialize(GLWrapper* gw) override;
	void cleanUp() override;

	const glm::mat4 getPVMatrix() const;
	const glm::mat4 getIPVMatrix() const;
	static const glm::vec2 getTranslation();
	static const float getZScaling();

private:
	GateRenderer gate_renderer;
	GridRenderer grid_renderer;

	glm::mat4 pview_mat;
	glm::mat4 ipview_mat;

	static float z_scl;
	static double lx;
	static double ly;

	Component* point = nullptr;
	Component* point_in = nullptr;
	Component* grid[20] = { nullptr };
	//NGateDef* gate[100] = { nullptr };
};

