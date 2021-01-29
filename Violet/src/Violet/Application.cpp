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
		m_vertexArray.reset(VertexArray::Create());	

		
		float vertices[] = {
		   -0.5f, -0.5f, 0.0f,    0.9f,  0.9f, 0.3f, 1.0f,
			0.5f, -0.5f, 0.0f,	  0.9f,  0.9f, 0.3f, 1.0f,
			0.0f,  0.5f, 0.0f,    0.9f,  0.3f, 0.3f, 1.0f

		};
		 
		std::shared_ptr<VertexBuffer>vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		
		VertexLayout layout = { {VertexAttributeDataType::Float3, "Position"},
								{VertexAttributeDataType::Float4, "Color"} };
		
		vertexBuffer->setLayout(layout);
		
		m_vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		//Create shaders
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_position;			
			layout(location = 1) in vec4 a_color;
			out vec4 v_color;
			
			void main(){
				v_color = a_color;
				gl_Position = a_position;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 o_color;			
			in vec4 v_color;
			
			void main(){
				o_color = v_color;
			}
		)";

		m_shader.reset(Shader::Create(vertexSrc, fragmentSrc));
		m_shader->bind();
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

			glDrawElements(GL_TRIANGLES, m_vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

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