#include <utility>
#include "Gui.h"

GLFWwindow* Gui::window = nullptr;

Gui::Gui(GLFWwindow* window)
{
	window_hash.rehash(30);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//Not required for now -> Using defaults
	ImGuiIO& io = ImGui::GetIO();

	//Setup dark style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::newFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::render() const
{
	static bool l = true;
	static float f = 0.0f;
	static double lt = glfwGetTime();
	static bool p_open = true;

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
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	
		if (ImGui::Begin("A", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("    [Statistics]");
			ImGui::Separator();
			ImGui::TextColored(ImColor(1.0f, 1.0f, 0.0f), "Framerate: %9.4f\n", io.Framerate);
			ImGui::TextColored(ImColor(1.0f, 0.0f, 1.0f), "Frametime: %9.4f\n", 1000.0f / io.Framerate);
		}
		ImGui::End();
	}

	for (auto window : window_hash)
	{
		window.second.first(window.second.second);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::pushWindow(std::function<bool*> body)
{
	static unsigned window_push_ct = 0;
	std::pair<std::function<bool*>, bool*> pair = std::make_pair(body, new bool(true));
	window_hash.emplace(window_push_ct++, pair);
}

void Gui::popWindow(unsigned id)
{
	delete window_hash.at(id).second;
	window_hash.erase(id);
}
