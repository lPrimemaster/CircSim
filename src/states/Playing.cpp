#include "Playing.h"
#include <typeinfo>

float Playing::y_scl = 0.0f;

void scroll_callback(GLFWwindow * window, double xoff, double yoff)
{
	//Cull the selection to a certain zoom only
	Playing::y_scl += (Playing::y_scl < 0 ? yoff : yoff < 0 ? yoff : 0);
}

void Playing::draw(GLWrapper* gw)
{
	renderer.render();
	Gui::Get().newFrame();
	Gui::Get().render();
}

void Playing::handle(GLWrapper* gw)
{
	//Handle looking at
	auto window = gw->getWindow();
	int rmb_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	static double last_cursor_x = 0.0;
	static double last_cursor_y = 0.0;

	static double lx = 0.0;
	static double ly = 0.0;

	if (rmb_state == GLFW_PRESS)
	{
		double new_cursor_x = 0.0;
		double new_cursor_y = 0.0;

		glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);

		double delta_x = last_cursor_x - new_cursor_x;
		double delta_y = last_cursor_y - new_cursor_y;

		float abs_y_scl_weighted = abs(y_scl) * 0.1f;

		double weight_x = delta_x * 0.001;
		double weight_y = delta_y * 0.001;

		//Weighted speed based on distance over z axis
		lx += weight_x * (abs_y_scl_weighted + 1.0);
		ly -= weight_y * (abs_y_scl_weighted + 1.0);

	}

	//Update aspect ratio
	glm::vec2 wh = gw->getWindowDim();
	pview_mat = glm::perspective(glm::radians(60.0f), wh.x / wh.y, 0.001f, 100.0f) * glm::lookAt(glm::vec3(lx, ly, 1.0f - y_scl * 0.05f), glm::vec3(lx, ly, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Get last cursor position before draw
	glfwGetCursorPos(window, &last_cursor_x, &last_cursor_y);
}

void Playing::update(GLWrapper* gw)
{
	auto registered_nodes = gw->getSimulation()->getRegisteredNodes();
	for (auto rn : registered_nodes)
	{
		for (int i = 0; i < 100; i++)
		{
			gate[i]->updateInput(rn.second->getState());
		}
	}

	renderer.setProjectViewMatrix(pview_mat);
}

void Playing::initialize(GLWrapper* gw)
{
	Gui::setContext(gw->getWindow());
	glfwSetScrollCallback(gw->getWindow(), scroll_callback);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2.5f);
	glPointSize(7.0f);

	Registry::registerAllBasicGeometry();

	point = new Component("Point");
	point->transform().update(glm::vec2(0.0f, 1.0f));
	point->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	point_in = new Component("Point");
	point_in->transform().update(glm::vec2(0.0f, 0.0f));
	point_in->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));


	for (int i = 0; i < 10; i++)
	{
		grid[i] = new Component("Line");
		grid[i]->transform().update(glm::vec2((float)i / 5.0f - 1.0f, 1.0f), glm::vec2((float)i / 5.0f - 1.0f, -1.0f));
		grid[i]->setColor(glm::vec4(0.2f));
		renderer.push(grid[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		grid[i + 10] = new Component("Line");
		grid[i + 10]->transform().update(glm::vec2(-1.0f, (float)i / 5.0f - 1.0f), glm::vec2(1.0f, (float)i / 5.0f - 1.0f));
		grid[i + 10]->setColor(glm::vec4(0.2f));
		renderer.push(grid[i + 10]);
	}

	for (int i = 0; i < 100; i++)
	{
		glm::vec2 in;
		glm::vec2 out;

		in.x = (float)i;
		in.y = 0.0f;

		out.x = (float)i;
		in.y = 1.0f;

		gate[i] = new NGateDef();
		gate[i]->update(in, out);
		renderer.pushList(gate[i]->getComponentList(), NGateDef::GetComponentListSize());
	}

	//renderer.push(point); //Out reference
	//renderer.push(point_in); //Center reference
}

void Playing::cleanUp()
{
	Registry::unregisterAllBasicGeometry();

	for (int i = 0; i < 20; i++)
	{
		delete grid[i];
	}

	for (int i = 0; i < 100; i++)
	{
		delete gate[i];
	}

	delete point;
	delete point_in;
}
