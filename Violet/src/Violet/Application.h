#pragma once
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Violet/ImGui/ImGuiLayer.h"
#include "Core/DeltaTime.h"
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResize(WindowResizeEvent& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		Window& getWindow() { return *m_window; }
	public:
		static Application& getApplicationInstance() { return *s_ApplicationInstance; }
	private:
		Scoped<Window> m_window;
		bool m_applicationRunning = true;
		bool m_windowMinimized = false;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;
		DeltaTime f_deltaTime;

	private:
		static Application* s_ApplicationInstance;
	};


	//To be defined in client.
	Application* createApplication();
}
