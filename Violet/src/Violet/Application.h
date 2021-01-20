#pragma once
#include "Core.h"
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);
	private:
		std::unique_ptr<Window> m_window;
		bool m_applicationRunning = true;
	};


	//To be defined in client.
	Application* createApplication();
}
