#include "VIOPCH.h"
#include "ImGuiLayer.h"

//#include "glad/glad.h" //imgui_impl_opengl3.h already includes glad.h
#include "backends/imgui_impl_opengl3.h"

namespace Violet {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
	{
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
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		
		// Setup Platform/Renderer backends
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::onDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

	}
	void ImGuiLayer::onUpdate()
	{	
	
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(1280, 720); //Hard coded for now for testing, TODO: get current window size from our Window.

		//// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//##Content##//
		static unsigned int count = 0;
		ImGui::Begin("ImGui Test!" );                         
		ImGui::Text("Counting %.3f",(float)count++);
		ImGui::End();

		static bool showWindow = true;
		ImGui::ShowDemoWindow(&showWindow);



		//// Rendering
		ImGui::Render();
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}
	void ImGuiLayer::onEvent(Event& event)
	{
		//VIO_CORE_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		//event.setEventHandleStatus(true); 
	}
}