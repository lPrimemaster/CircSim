#include "Playing.h"
#include <typeinfo>

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
		static constexpr float DISTANCE = 10.0f;
		static constexpr int corner = 0;

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.35f);


		if (ImGui::Begin("DebugWindow", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			glm::vec2 mouse_pos = glm::vec2(io.MousePos.x, io.MousePos.y);
			glm::vec2 mouse_world_pos = playing->frame_mouse_pos;

			ImGui::Text("    [Statistics]");
			ImGui::Separator();
			ImGui::Text("Game");
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "Gate #: %u\n", playing->gate_tracker.size());

			ChunkCoord cc = ChunkManager::getChunkIndexAtPosition(mouse_world_pos);

			ImGui::Separator();
			ImGui::Text("Engine");
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "Framerate: %9.4f\n", io.Framerate);
			ImGui::TextColored(ImColor(1.0f, 0.0f, 1.0f), "Frametime: %9.4f\n", 1000.0f / io.Framerate);
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "ScreenScale: %9.4f\n", playing->getZScaling());
			ImGui::TextColored(ImColor(1.0f, 0.0f, 1.0f), "ScreenSize: <%9.4f> <%9.4f>\n", io.DisplaySize.x, io.DisplaySize.y);
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "LineCardinality: <%u>\n", playing->card);
			ImGui::TextColored(ImColor(1.0f, 0.0f, 1.0f), "Chunk: <%p>\n", ChunkManager::getChunkAtPosition(mouse_world_pos));
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "Chunk Index: <%d> <%d>\n", cc.chunk_id_x, cc.chunk_id_y);

			ImGui::Separator();
			ImGui::Text("Coordinates");
			ImGui::TextColored(ImColor(0.0f, 1.0f, 1.0f), "World Pos: <%9.4f> <%9.4f>\n", mouse_world_pos.x, mouse_world_pos.y);
			ImGui::TextColored(ImColor(0.0f, 1.0f, 1.0f), "Screen Pos: <%9.4f> <%9.4f>\n", mouse_pos.x, mouse_pos.y);

			ImGui::Separator();
			auto stats = Geometry::getStatistics();
			ImGui::Text("Geometry Registry");
			ImGui::TextColored(ImColor(0.2f, 0.7f, 0.3f), "Bucket count: <%d>\n", stats.first);
			ImGui::TextColored(ImColor(0.2f, 0.7f, 0.3f), "Memory (kbytes): <%2.2f>\n", stats.second / 1024.0f);
		}
		ImGui::End();
	}
}
#endif

float Playing::z_scl = 0.0f;
double Playing::lx = 0.0;
double Playing::ly = 0.0;

void Playing::draw(GLWrapper* gw)
{
	grid_renderer.render();
	gate_renderer.render();
	gui_renderer.render();
	DebugGui::Get().newFrame();
	DebugGui::Get().render(gw->getWindow(), this);
}

void Playing::handle(GLWrapper* gw)
{
	//Handle looking at
	auto window = gw->getWindow();
	int rmb_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	glm::vec2 wh = gw->getWindowDim();

	auto physics = gw->getPhysics();

	//This scaling configuration adds 0.2 to X with in world space every zscale unit
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

	card = grid_renderer.updateGrid(abs_y_scl_weighted, glm::vec2(lx, ly), wh);

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

	//Calculate the mouse position for this frame
	frame_mouse_pos = math::screenToWorld(wh, glm::vec2(last_cursor_x, last_cursor_y), ipview_mat);

	physics->setMouseAtomic(frame_mouse_pos);
	physics->setCurrentActiveChunkAtomic(ChunkManager::getChunkAtPosition(frame_mouse_pos));


	if (last_gates.not_gate != nullptr)
	{
		last_gates.not_gate->update(last_gates.last_pos, math::snapToGrid(frame_mouse_pos, 0.2f, 0.2f));
		
	}
	else if (last_gates.switch_gate != nullptr)
	{
		last_gates.switch_gate->update(last_gates.last_pos, math::snapToGrid(frame_mouse_pos, 0.2f, 0.2f));
	}
}

void Playing::update(GLWrapper* gw)
{
	if (!require_update.empty())
	{
		for (auto re : require_update)
		{
			gw->getSimulation()->registerNode(re);
		}
		require_update.clear();
	}

	//TODO: Why not a callback later
	for (auto g : gate_tracker)
	{
		if (g->childType() == typeid(SwitchGate))
		{
			SwitchGate* sg = g->convert<SwitchGate>();
			if(sg->getInputs()[0]->node != nullptr)
				sg->updateInput(sg->getInputs()[0]->node->getState());
		}
		else
		{
			NotGate* ng = g->convert<NotGate>();
			if(ng->getInputs()[0]->node != nullptr)
				ng->updateInput(ng->getInputs()[0]->node->getState());
		}
	}

	glm::vec2 wDims = gw->getWindowDim();
	static glm::vec2 last = wDims;

	grid_renderer.setPVMatrix(pview_mat);
	gate_renderer.setPVMatrix(pview_mat);
	gui_renderer.setPVMatrix(glm::ortho(0.0f, wDims.x, 0.0f, wDims.y)); //This is only the projection matrix instead

	//Call everything that should reajust with the viewport resize
	if (wDims != last)
	{
		gui_renderer.adjustWindows(GUI_LOCK_RIGHT);
		last = wDims;
	}
}

void Playing::scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
	//Cull the selection to a certain zoom only
	Playing::z_scl += (Playing::z_scl < 0 ? yoff : yoff < 0 ? yoff : 0);
}

//TODO: Remember to check if the mouse is on a legal location
//TODO: Remember to check if the gate is on a legal location
//TODO: Create a lower alpha of the gate while on placemente mode (key not released)
//TODO: Don't hard code grid spacings, as well as the gate min length draw check
void Playing::click_callback(GLFWwindow* window, int button, int action, int mods)
{
	static int lastButtonClick = button;
	static glm::vec2 initial_pos;
	static bool is_pressed = false;

	//Check if button is inside a gate -> activate interaction
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		Chunk* curr_chunk = ChunkManager::getChunkAtPosition(frame_mouse_pos);
		if (curr_chunk != nullptr)
		{
			for (auto ic : curr_chunk->getInteractorsList())
			{
				if (ic->checkMouse(frame_mouse_pos))
				{
					ic->onClick();
					break; //Admit gates don't overlap
				}
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		initial_pos = math::snapToGrid(frame_mouse_pos, 0.2f, 0.2f);
		is_pressed = true;
		
		if (!(mods & GLFW_MOD_SHIFT))
		{
			last_gates.not_gate = new NotGate();
			last_gates.not_gate->changeAlpha(0.7f);
			gate_renderer.pushList(last_gates.not_gate->getComponentList(), last_gates.not_gate->getComponentListSize());
			gate_tracker.push_back(last_gates.not_gate);
		}
		else
		{
			last_gates.switch_gate = new SwitchGate();
			last_gates.switch_gate->changeAlpha(0.7f);
			gate_renderer.pushList(last_gates.switch_gate->getComponentList(), last_gates.switch_gate->getComponentListSize());
			gate_tracker.push_back(last_gates.switch_gate);
		}

		last_gates.last_pos = initial_pos;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && is_pressed)
	{
		is_pressed = false;
		glm::vec2 pos_release_snap = math::snapToGrid(frame_mouse_pos, 0.2f, 0.2f);
		if (glm::distance(initial_pos, pos_release_snap) >= 0.2f * 3.0f) //TODO: Check if this works on the math.h -> comparing floating points
		{
			Chunk* curr_chunk = ChunkManager::getChunkAtPosition(frame_mouse_pos);
			if (!curr_chunk)
			{
				ChunkManager::createChunkAtPosition(frame_mouse_pos);
				curr_chunk = ChunkManager::getChunkAtPosition(frame_mouse_pos);
			}

			if (!(mods & GLFW_MOD_SHIFT))
			{
				GateManager::addGate(last_gates.not_gate);
				last_gates.not_gate->isFixed() = true;
				last_gates.not_gate->changeAlpha(1.0f);
				last_gates.not_gate->update(initial_pos, pos_release_snap);

				curr_chunk->insertGate(last_gates.not_gate);

				require_update = ChunkManager::updateConnectorNode(last_gates.not_gate->getOutputs()[0]);
				last_gates.not_gate = nullptr;
			}
			else
			{
				GateManager::addGate(last_gates.switch_gate);
				last_gates.switch_gate->isFixed() = true;
				last_gates.switch_gate->changeAlpha(1.0f);
				last_gates.switch_gate->update(initial_pos, pos_release_snap);

				curr_chunk->insertGate(last_gates.not_gate);

				auto ucn = ChunkManager::updateConnectorNode(last_gates.switch_gate->getOutputs()[0]);
				auto uicn = ChunkManager::updateInteractConnectorNode(dynamic_cast<InteractConnector*>(last_gates.switch_gate->getInputs()[0]));
				require_update.insert(require_update.end(), ucn.begin(), ucn.end());
				require_update.insert(require_update.end(), uicn.begin(), uicn.end());
				last_gates.switch_gate = nullptr;
			}
		}
		else
		{
			if (last_gates.not_gate != nullptr)
			{
				gate_renderer.popList(last_gates.not_gate->getComponentList(), last_gates.not_gate->getComponentListSize());
				gate_tracker.pop_back();
				delete last_gates.not_gate;
				last_gates.not_gate = nullptr;
			}
			else if(last_gates.switch_gate != nullptr)
			{
				gate_renderer.popList(last_gates.switch_gate->getComponentList(), last_gates.switch_gate->getComponentListSize());
				gate_tracker.pop_back();
				delete last_gates.switch_gate;
				last_gates.switch_gate = nullptr;
			}
		}
	}

	lastButtonClick = button;
}

void Playing::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Z && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL))
	{
		if (gate_tracker.size() > 0)
		{
			auto it = gate_tracker.end() - 1;
			gate_renderer.popList((*it)->getComponentList(), (*it)->getComponentListSize());
			delete *it;
			gate_tracker.pop_back();
		}
	}
}

void Playing::initialize(GLWrapper* gw)
{
	ChunkManager::allocateStart();

	StateInfo si;
	si.state = this;
	si.derived_state = PLAYING;

	DebugGui::setContext(gw->getWindow());
	InputManager::setActiveWindow(gw->getWindow());
	InputManager::setCallbackControllerState(si);

	gui_renderer.addWindow(new GuiWindow());

	glLineWidth(2.5f);
	glPointSize(7.0f);

#ifdef DEBUG
	DebugGui::Get().pushWindow(debug);

	auto lamb = [&](bool* close, GLFWwindow* window, State* state)
	{
		static constexpr float DISTANCE = 10.0f;
		static constexpr int corner = 1;

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		if (ImGui::Begin("GridRenderer Values", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Line values\n");
			ImGui::SliderFloat("Width [lw]", &grid_renderer.getLW(), 0.0f, 0.01f, "%.5f");
		}
		ImGui::End();
	};

	DebugGui::Get().pushWindow(lamb);
#endif // DEBUG
}

void Playing::cleanUp()
{
	for (auto gate : gate_tracker)
	{
		delete gate;
	}

	ChunkManager::deleteChunkAtPosition(glm::vec2(0, 0));
}

const glm::mat4 Playing::getPVMatrix() const
{
	return pview_mat;
}

const glm::mat4 Playing::getIPVMatrix() const
{
	return ipview_mat;
}

const glm::vec2 Playing::getMousePositionWorldSpace() const
{
	return frame_mouse_pos;
}

const glm::vec2 Playing::getTranslation()
{
	return glm::vec2(lx, ly);
}

const float Playing::getZScaling()
{
	return z_scl;
}
