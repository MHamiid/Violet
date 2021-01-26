#include "VIOPCH.h"
#include "Application.h"
#include "glad/glad.h"

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
		
		m_window = std::unique_ptr<Window>(Window::Create(WindowProperties("Violet Engine", 1280, 720 , true , std::bind(&Application::onEvent, this, std::placeholders::_1))));
		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);
		
		//Test OpenGL
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		float vertices[3 * 3] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f

		};
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	Application::~Application()
	{
		
	}
	void Application::onEvent(Event& event) {

		//VIO_DEBUG(event.getName());
		EventDispatcher dispatcher(event);	
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1)); //Dispatch all window close events to onWindowClose function

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

			//Test OpenGl 
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			//Iterate over all the layers
			for (Layer* layer : m_layerStack) {
			
				layer->onUpdate();
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