#include "DebugGui.h"

GLFWwindow* DebugGui::window = nullptr;

DebugGui::DebugGui(GLFWwindow* window)
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

DebugGui::~DebugGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void DebugGui::newFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void DebugGui::render(GLFWwindow* window, State* state) const
{
	for (auto w : window_hash)
	{
		if(*w.second.second)
			w.second.first(w.second.second, window, state);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGui::pushWindow(std::function<void(bool*, GLFWwindow*, State*)> body)
{
	static unsigned window_push_ct = 0;
	auto pair = std::make_pair(body, new bool(true));
	window_hash.emplace(window_push_ct++, pair);
}

void DebugGui::popWindow(unsigned id)
{
	delete window_hash.at(id).second;
	window_hash.erase(id);
}

bool* DebugGui::getWindowState(unsigned id)
{
	return window_hash.at(id).second;
}
