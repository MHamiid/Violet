#pragma once
#include "Window.h"
#include "Violet/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Violet/ImGui/ImGuiLayer.h"
#include "DeltaTime.h"

int main(int argc, char** argv);
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResize(WindowResizeEvent& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		Window& getWindow() { return *m_window; }
	private:
		//Application main run loop
		void run();
		friend int ::main(int argc, char** argv);  /* Application's run loop private function is accessible in main function, 
												    * which is handled by EntryPoint header but not in client side's Application child class. 
													* Therefore calling run function is handled only by the engine side.*/
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
