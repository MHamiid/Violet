#pragma once
#include "Core.h"
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Violet/ImGui/ImGuiLayer.h"
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
		static Application& getApplication() { return *s_ApplicationInstance; }
	private:
		std::unique_ptr<Window> m_window;
		bool m_applicationRunning = true;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;

	private:
		static Application* s_ApplicationInstance;
	};


	//To be defined in client.
	Application* createApplication();
}
