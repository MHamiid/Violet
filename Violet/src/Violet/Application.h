#pragma once
#include "Core.h"
#include "Window.h"
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_applicationRunning = true;
	};


	//To be defined in client.
	Application* createApplication();
}
