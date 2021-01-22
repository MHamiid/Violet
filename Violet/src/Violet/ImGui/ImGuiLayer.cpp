#include "VIOPCH.h"
#include "ImGuiLayer.h"
#include "Violet/Application.h"

#include "backends/imgui_impl_glfw.h"
//#include "glad/glad.h" //imgui_impl_opengl3.h already includes glad.h
#include "backends/imgui_impl_opengl3.h"

namespace Violet {
	ImGuiLayer* ImGuiLayer::s_ImGuiLayer = nullptr;
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") 
	{
		//ImGuiLayer should have only one instance
		VIO_CORE_ASSERT(!s_ImGuiLayer, "ImGuiLayer Already Created!!!");
		s_ImGuiLayer = this; //Our single instance of ImGuiLayer
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::onAttach()
	{
	
	   //Setup Dear ImGui context
		VIO_CORE_INFO("Initializing ImGUI: {0}!", IMGUI_CHECKVERSION());
		ImGui::CreateContext();
    	ImGuiIO& io = ImGui::GetIO(); //(void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		
		// Setup Platform/Renderer backends
		ImGui_ImplOpenGL3_Init("#version 410");

		//Get window from the Application instance.
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}
	void ImGuiLayer::onDetach()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::onImGuiRender()
	{
		//##Content##//
		ImGui::Begin("ImGui Test!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		static bool showWindow = true;
		ImGui::ShowDemoWindow(&showWindow);
	}
	
	void ImGuiLayer::onEvent(Event& event)
	{
		//VIO_CORE_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		//event.setEventHandleStatus(true); 
	}
	void ImGuiLayer::beginImGuiFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}
	void ImGuiLayer::endImGuiFrame()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}