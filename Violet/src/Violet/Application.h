#pragma once
#include "Core.h"
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
#include "LayerStack.h"
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
		void pushOverlayer(Layer* layer);
	private:
		std::unique_ptr<Window> m_window;
		bool m_applicationRunning = true;
		LayerStack m_layerStack;
	};


	//To be defined in client.
	Application* createApplication();
}
