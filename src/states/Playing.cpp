#include "Playing.h"
#include <typeinfo>

NGateDef* gate[1] = { nullptr };

#define DEBUG

#ifdef DEBUG
static void debug(bool* close, GLFWwindow* window, State* state)
{
	static bool l = true;
	static float f = 0.0f;
	static double lt = glfwGetTime();
	static bool p_open = true;

	Playing* playing = reinterpret_cast<Playing*>(state);

	int k0 = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	int k1 = glfwGetKey(window, GLFW_KEY_F3);

	if (k0 == GLFW_PRESS && k1 == GLFW_PRESS && glfwGetTime() - lt > 1)
	{
		p_open = !p_open;
		lt = glfwGetTime();
	}

	if (p_open)
	{
		const float DISTANCE = 10.0f;
		static int corner = 0;

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.35f);


		if (ImGui::Begin("DebugWindow", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("    [Statistics]");
			ImGui::Separator();
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "Framerate: %9.4f\n", io.Framerate);
			ImGui::TextColored(ImColor(1.0f, 0.0f, 1.0f), "Frametime: %9.4f\n", 1000.0f / io.Framerate);

			glm::vec2 mouse_pos = glm::vec2(io.MousePos.x, io.MousePos.y);

			ImGui::Separator();

			glm::vec2 mouse_world_pos = math::screenToWorld(mouse_pos, playing->getIPVMatrix());

			ImGui::TextColored(ImColor(0.0f, 1.0f, 1.0f), "World  Pos: <%9.4f> | <%9.4f>\n", mouse_world_pos.x, mouse_world_pos.y);
			ImGui::TextColored(ImColor(0.0f, 1.0f, 1.0f), "Screen Pos: <%9.4f> | <%9.4f>\n", mouse_pos.x, mouse_pos.y);

			ImGui::Separator();

			ImGui::TextColored(ImColor(0.7f, 0.2f, 0.0f), "Line Width: <%9.4f>\n", -0.250f / (0.05f * (playing->getZScaling() - 1.0f)));
		}
		ImGui::End();
	}
}
#endif

float Playing::z_scl = 0.0f;
double Playing::lx = 0.0;
double Playing::ly = 0.0;

void scroll_callback(GLFWwindow * window, double xoff, double yoff)
{
	//Cull the selection to a certain zoom only
	Playing::z_scl += (Playing::z_scl < 0 ? yoff : yoff < 0 ? yoff : 0);
}

void Playing::draw(GLWrapper* gw)
{
	grid_renderer.render();
	gate_renderer.render();
	Gui::Get().newFrame();
	Gui::Get().render(gw->getWindow(), this);
}

void Playing::handle(GLWrapper* gw)
{
	//Handle looking at
	auto window = gw->getWindow();
	int rmb_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	glm::vec2 wh = gw->getWindowDim();

	static double last_cursor_x = 0.0;
	static double last_cursor_y = 0.0;

	float abs_y_scl_weighted = abs(z_scl) * 0.1f;

	if (rmb_state == GLFW_PRESS)
	{
		double new_cursor_x = 0.0;
		double new_cursor_y = 0.0;

		glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);

		double delta_x = last_cursor_x - new_cursor_x;
		double delta_y = last_cursor_y - new_cursor_y;

		double weight_x = delta_x * 2.0 / wh.x;
		double weight_y = delta_y * 2.0 / wh.x;

		//Weighted speed based on distance over "z" axis
		lx += weight_x * (abs_y_scl_weighted + 1.0);
		ly -= weight_y * (abs_y_scl_weighted + 1.0);
	}

	point->transform().update(glm::vec2(lx, ly));

	//Update aspect ratio and scales
	float aspect = (wh.x / wh.y);
	glm::vec3 weighted_scale = glm::vec3(1.0f / (1.0f + abs_y_scl_weighted), aspect / (1.0f + abs_y_scl_weighted), 0.0f);
	glm::vec3 inv_wscale = glm::vec3(1.0f + abs_y_scl_weighted, (1.0f + abs_y_scl_weighted) / aspect, 0.0f);

	//Write view matrix
	pview_mat = glm::scale(glm::mat4(), weighted_scale) * glm::translate(glm::mat4(), glm::vec3(-lx, -ly, 0.0f));
	ipview_mat = glm::translate(glm::mat4(), glm::vec3(lx, ly, 0.0f)) * glm::scale(glm::mat4(), inv_wscale);

	//Get last cursor position before draw
	glfwGetCursorPos(window, &last_cursor_x, &last_cursor_y);

	//Keep mouse inside the screen when dragging the scene
	if (rmb_state == GLFW_PRESS)
	{
		bool over_x = last_cursor_x > wh.x;
		bool over_y = last_cursor_y > wh.y;

		//TODO: check against 0.0 equality and use delta value to decide 
		bool under_x = last_cursor_x < 0.0;
		bool under_y = last_cursor_y < 0.0;

		bool mchanged = over_x || over_y || under_x || under_y;

		if (mchanged)
		{
			last_cursor_x = over_x ? 0.0 : under_x ? wh.x : last_cursor_x;
			last_cursor_y = over_y ? 0.0 : under_y ? wh.y : last_cursor_y;

			glfwSetCursorPos(window, last_cursor_x, last_cursor_y);
		}
	}
}

void Playing::update(GLWrapper* gw)
{
	auto registered_nodes = gw->getSimulation()->getRegisteredNodes();
	for (auto rn : registered_nodes)
	{
		for (int i = 0; i < 1; i++)
		{
			gate[i]->updateInput(rn.second->getState()); //Batch this later for speed
		}
	}


	grid_renderer.setPVMatrix(pview_mat);
	gate_renderer.setPVMatrix(pview_mat);
}

void Playing::initialize(GLWrapper* gw)
{
	Gui::setContext(gw->getWindow());
	glfwSetScrollCallback(gw->getWindow(), scroll_callback);

	glLineWidth(2.5f);
	glPointSize(7.0f);

	point = new Component("Point");
	point->transform().update(glm::vec2(0.0f, 0.0f));
	point->setColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	point_in = new Component("Point");
	point_in->transform().update(glm::vec2(0.0f, 0.0f));
	point_in->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));


	for (int i = 0; i < 10; i++)
	{
		grid[i] = new Component("Line");
		grid[i]->transform().update(glm::vec2((float)i / 5.0f - 1.0f, 1.0f), glm::vec2((float)i / 5.0f - 1.0f, -1.0f));
		grid[i]->setColor(glm::vec4(0.5f));
		grid_renderer.push(grid[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		grid[i + 10] = new Component("Line");
		grid[i + 10]->transform().update(glm::vec2(-1.0f, (float)i / 5.0f - 1.0f), glm::vec2(1.0f, (float)i / 5.0f - 1.0f));
		grid[i + 10]->setColor(glm::vec4(0.5f));
		grid_renderer.push(grid[i + 10]);
	}

	for (int i = 0; i < 1; i++)
	{
		glm::vec2 in;
		glm::vec2 out;

		in.x = (float)i;
		in.y = 0.0f;

		out.x = (float)i;
		out.y = 1.0f;

		gate[i] = new NGateDef();
		gate[i]->update(in, out);
		gate_renderer.pushList(gate[i]->getComponentList(), NGateDef::GetComponentListSize());
	}

#ifdef DEBUG
	Gui::Get().pushWindow(debug);

	auto lamb = [&](bool* close, GLFWwindow* window, State* state)
	{
		if (ImGui::Begin("GridRenderer Values"))
		{
			ImGui::Text("Line values\n");
			ImGui::SliderFloat("lw", &grid_renderer.getLW(), 0.0f, 0.01f, "%.5f");

			GLint range[2];
			glGetIntegerv(GL_LINE_WIDTH_RANGE, range); // or GL_ALIASED_LINE_WIDTH_RANGE

			ImGui::Text("Max line width: [%d, %d]\n", range[0], range[1]);
		}
		ImGui::End();
	};

	Gui::Get().pushWindow(lamb);
#endif

	//renderer.push(point); //Mid screen ref
	//renderer.push(point_in); //Center reference
}

void Playing::cleanUp()
{
	for (int i = 0; i < 20; i++)
	{
		delete grid[i];
	}

	for (int i = 0; i < 1; i++)
	{
		delete gate[i];
	}

	delete point;
	delete point_in;
}

const glm::mat4 Playing::getPVMatrix() const
{
	return pview_mat;
}

const glm::mat4 Playing::getIPVMatrix() const
{
	return ipview_mat;
}

const glm::vec2 Playing::getTranslation()
{
	return glm::vec2(lx, ly);
}

const float Playing::getZScaling()
{
	return z_scl;
}
