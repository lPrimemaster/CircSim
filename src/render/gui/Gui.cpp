#include "Gui.h"

GLFWwindow* Gui::window = nullptr;

Gui::Gui(GLFWwindow* window)
{
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
	bool l = true;
	float f;

	ImGui::Begin("Hello, world!");

	ImGui::Text("This is some useful text.");
	ImGui::Checkbox("Demo Window", &l);
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
