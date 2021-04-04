#include "VIOPCH.h"
#include "ImGuiLayer.h"
#include "Violet/Core/Application.h"

#include <backends/imgui_impl_glfw.h>
//#include "glad/glad.h" //imgui_impl_opengl3.h already includes glad.h
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

namespace Violet {
	ImGuiLayer* ImGuiLayer::s_ImGuiLayer = nullptr;
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") 
	{
		//ImGuiLayer should have only one instance
		VIO_CORE_ASSERT(!s_ImGuiLayer, "ImGuiLayer Already Created!!!");
		s_ImGuiLayer = this; //Our single instance of ImGuiLayer
	}
	void ImGuiLayer::onAttach()
	{
	
	   //Setup Dear ImGui context
		VIO_CORE_INFO("Initializing ImGUI: {0}!", IMGUI_CHECKVERSION());
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		
		io.Fonts->AddFontFromFileTTF("assets/fonts/SourceSansPro/SourceSansPro-Bold.ttf", 18.0f);  //Font at index 0
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/SourceSansPro/SourceSansPro-Regular.ttf", 18.0f);  //Font at index 1


		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}



		// Setup Platform/Renderer backends
		ImGui_ImplOpenGL3_Init("#version 410");

		//Get window from the Application instance.
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplicationInstance().getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}
	void ImGuiLayer::onDetach()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::onEvent(Event& event)
	{
		//VIO_CORE_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		//event.setEventHandleStatus(true); 
		if (m_blockImGuiEvents) {
		/*Let ImGui set mouse or keyboard events to handled whenever ImGui receive these events*/
			ImGuiIO& io = ImGui::GetIO();

			bool handledStatus = event.getEventHandleStatus();

			handledStatus |= event.isInCategory(EventCategory::Mouse) & io.WantCaptureMouse;
			handledStatus |= event.isInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard;

			event.setEventHandleStatus(handledStatus);
		}
	}
	void ImGuiLayer::BeginImGuiFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}
	void ImGuiLayer::EndImGuiFrame()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		ImGuiIO& io = ImGui::GetIO();
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}