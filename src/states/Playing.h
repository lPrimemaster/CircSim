#pragma once
#include <iostream>
#include "State.h"
#include "../render/Renderer.h"
#include "../gates/NGateDef.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../render/gui/Gui.h"

#include "../Registry.h"

class Playing : public State
{
public:
	Playing() = default;
	~Playing() = default;

	friend void scroll_callback(GLFWwindow* window, double xoff, double yoff);

	void draw(GLWrapper* gw) override;
	void handle(GLWrapper* gw) override;
	void update(GLWrapper* gw) override;

	void initialize(GLWrapper* gw) override;
	void cleanUp() override;

private:
	Renderer renderer;

	glm::mat4 pview_mat = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	static float y_scl;

	Component* point = nullptr;
	Component* point_in = nullptr;
	Component* grid[20] = { nullptr };

	NGateDef* gate[100] = { nullptr };
};

