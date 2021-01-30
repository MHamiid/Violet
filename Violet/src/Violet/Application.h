#pragma once
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Violet/ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h" //Temp
#include "Renderer/VertexArray.h" //Temp
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		Window& getWindow() { return *m_window; }
	public:
		static Application& getApplicationInstance() { return *s_ApplicationInstance; }
	private:
		std::unique_ptr<Window> m_window;
		bool m_applicationRunning = true;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;

	private:
		static Application* s_ApplicationInstance;
		std::shared_ptr<Shader> m_shader;	//Temp
		std::shared_ptr<VertexArray> m_vertexArray; //Temp
	};


	//To be defined in client.
	Application* createApplication();
}
