#include "VIOPCH.h"
#include "Application.h"
#include "Renderer/Renderer.h"

namespace Violet {
	/*bool eventHandling(MouseButtonPressedEvent& ev) {
		VIO_INFO("Event Handle Function Called");
		return true;
	}*/
	Application* Application::s_ApplicationInstance = nullptr;
	Application::Application()
	{	
		VIO_CORE_ASSERT(!s_ApplicationInstance, "Application Already Created!!!");
		s_ApplicationInstance = this; //Our single instance of application
		
		m_window = Scoped<Window>(Window::Create(WindowProperties("Violet Engine", 1280, 720 , true , VIO_BIND_EVENT_FUNCTION(Application::onEvent))));
		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);
		
		Renderer::Init();

	}
	Application::~Application()
	{
	
	}
	void Application::onEvent(Event& event) {

		//VIO_DEBUG(event.getName());
		EventDispatcher dispatcher(event);	
		dispatcher.dispatch<WindowCloseEvent>(VIO_BIND_EVENT_FUNCTION(Application::onWindowClose)); //Dispatch all window close events to onWindowClose function
		dispatcher.dispatch<WindowResizeEvent>(VIO_BIND_EVENT_FUNCTION(Application::onWindowResize));

		//Events that are handled by layers

		//Iterate through the layer backward to handle events
		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
		
			if (event.getEventHandleStatus()) { //Check if the event has been handled by the layer
				break; //If the layer handled the event, break and don't propagate the event to the remaining layers
			}
			(*--it)->onEvent(event); //Send the event to that layer
		}

	}
	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		m_applicationRunning = false;
		return true; //Confirm that the evet has been handled 
	}
	bool Application::onWindowResize(WindowResizeEvent& event)
	{
		Renderer::OnWindowResize(event.getWidth(), event.getHeight());
		if (event.getWidth() == 0 || event.getHeight() == 0) {   //If the window is minimized, //NOTE: when window is minimized the width and height are set to zero

			m_windowMinimized = true;
			return false;
		}

		m_windowMinimized = false;
		
		return false;  //Don't block the event
	}
	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}
	void Application::pushOverlay(Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
	}
	void Application::run()
	{	

		while (m_applicationRunning) {

			//Calculate delta time
			f_deltaTime.calculateDeltaTime(m_window->getCurrentTime());

			//Don't update if the window is minimized
			//NOTE: We still update ImGuiLayer incase if ImGui is docked for example and still visible even when window is minimized
			if (!m_windowMinimized) {
				//Iterate over all the layers
				for (Layer* layer : m_layerStack) {

					layer->onUpdate(f_deltaTime);
				}
			}

			//Rendering ImGui stuff
			ImGuiLayer::BeginImGuiFrame(); //Start ImGui frame
			//Iterate over layers onImGuiRender content
			for (Layer* layer : m_layerStack) {
				layer->onImGuiRender();
			}

			ImGuiLayer::EndImGuiFrame();   //End ImGui frame
			m_window->onUpdate();	
		}
	}

	
}