#include <Violet.h>


//Violet::Application* Violet::createApplication();
//
//int main(int argc, char** argv) {
//
//	Violet::Application* app = Violet::createApplication();
//	app->run();
//	delete app;
//
//
//}

class GameLayer : public Violet::Layer {

public:
	GameLayer(const std::string layerDebugName = "Layer") : Layer(layerDebugName), m_camera(-1.0f, 1.0f, -1.0f, 1.0f) {
		m_camera.setPosition(glm::vec3(0.1f, 0.0f, 0.4f));
		m_camera.setRotationZ(90.0f);
		m_vertexArray.reset(Violet::VertexArray::Create());


		float vertices[] = {
		   -0.5f, -0.5f, 0.0f,    0.9f,  0.9f, 0.3f, 1.0f,
			0.5f, -0.5f, 0.0f,	  0.9f,  0.9f, 0.3f, 1.0f,
			0.0f,  0.5f, 0.0f,    0.9f,  0.3f, 0.3f, 1.0f

		};

		std::shared_ptr<Violet::VertexBuffer>vertexBuffer;
		vertexBuffer.reset(Violet::VertexBuffer::Create(vertices, sizeof(vertices)));


		Violet::VertexLayout layout = { {Violet::VertexAttributeDataType::Float3, "Position"},
								{Violet::VertexAttributeDataType::Float4, "Color"} };

		vertexBuffer->setLayout(layout);

		m_vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<Violet::IndexBuffer> indexBuffer;
		indexBuffer.reset(Violet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		//Create shaders
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_position;			
			layout(location = 1) in vec4 a_color;
			uniform mat4 u_viewProjection;

			out vec4 v_color;
			
			void main(){
				v_color = a_color;
				gl_Position = u_viewProjection * a_position;
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

		m_shader.reset(Violet::Shader::Create(vertexSrc, fragmentSrc));
	}
	
	void onUpdate() override {

		Violet::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Violet::RenderCommand::Clear();

		Violet::Renderer::BeginScene(m_camera);

		Violet::Renderer::Submit(m_shader, m_vertexArray);

		Violet::Renderer::EndScene();

	
		//VIO_INFO("{0} OnUpdate" , getName());
		//Basic Tests for inputs
		/*if (Violet::Input::IsKeyPressed(Violet::Key::A))
			VIO_TRACE("Key A is Pressed");
		if (Violet::Input::IsMouseButtonPressed(Violet::Mouse::BUTTON_1)) {
			auto [x, y] = Violet::Input::GetMousePosition();
			VIO_TRACE("Mouse Button 1 is Pressed at position {0}, {1}", x, y);
		}
		if (Violet::Input::IsMouseButtonPressed(Violet::Mouse::BUTTON_2)) {
			auto [x, y] = Violet::Input::GetMousePosition();
			VIO_TRACE("Mouse Button 2 is Pressed at position {0}, {1}", x, y);
		}*/
	}

	void onEvent(Violet::Event& event) override {
		//VIO_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		//event.setEventHandleStatus(true); 
		/*if (event.getEventType() == Violet::EventType::KeyPressed) {
			Violet::KeyPressedEvent& keyEvent = static_cast<Violet::KeyPressedEvent&>(event);
			VIO_TRACE("Key {0} is pressed", (char)keyEvent.getKeyCode());
		
		}*/
	}
private:
	std::shared_ptr<Violet::Shader> m_shader;
	std::shared_ptr<Violet::VertexArray> m_vertexArray;
	Violet::OrthographicCamera m_camera;
};

class SandBox : public Violet::Application {

public:
	SandBox() {

		pushLayer(new GameLayer("First Layer"));
		//pushLayer(new GameLayer("Second Layer"));
	}

};

Violet::Application* Violet::createApplication() {
    
	
	
	return new SandBox();


}