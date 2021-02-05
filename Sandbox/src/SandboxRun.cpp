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
	GameLayer(const std::string layerDebugName = "Layer") : Layer(layerDebugName), m_camera(-1.0f, 1.0f, -1.0f, 1.0f),
		m_cameraPosition(0.0f, 0.0f, 0.0f), m_cameraRotation(0.0f), m_objectPosition(0.0f, 0.0f, 0.0f){
		
		m_vertexArray.reset(Violet::VertexArray::Create());


		/*float vertices[] = {
		   -0.5f, -0.5f, 0.0f,    0.9f,  0.9f, 0.3f, 1.0f,
			0.5f, -0.5f, 0.0f,	  0.9f,  0.9f, 0.3f, 1.0f,
			0.0f,  0.5f, 0.0f,    0.9f,  0.3f, 0.3f, 1.0f

		};*/

		float vertices[] = {
		   -0.5f, -0.5f, 0.0f,    0.0f,  0.0f,
			0.5f, -0.5f, 0.0f,	  1.0f,  0.0f,
			0.5f,  0.5f, 0.0f,    1.0f,  1.0f,
		   -0.5f,  0.5f, 0.0f,    0.0f,  1.0f
		};


		Violet::Ref<Violet::VertexBuffer>vertexBuffer;
		vertexBuffer.reset(Violet::VertexBuffer::Create(vertices, sizeof(vertices)));


		Violet::VertexLayout layout = { {Violet::VertexAttributeDataType::Float3, "Position"},
								{Violet::VertexAttributeDataType::Float2, "TexCoord"} };

		vertexBuffer->setLayout(layout);

		m_vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		Violet::Ref<Violet::IndexBuffer> indexBuffer;
		indexBuffer.reset(Violet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);



		m_shader.reset(Violet::Shader::Create("assets/shaders/Texture.glsl"));
		m_texture = Violet::Texture2D::Create("assets/textures/CheckerBoard_RGB.png");
		m_transparentTexture = Violet::Texture2D::Create("assets/textures/LetterV_RGBA.png");
		m_shader->bind();
		m_shader->setInt("u_texture", 0); //Which texture slot to use
	}
	
	void onUpdate(Violet::DeltaTime& deltaTime) override {
		//VIO_TRACE("Delta Time: {0} ({1}ms)",deltaTime.getSeconds(), deltaTime.getMilliSeconds());

		//Camera controls
		if (Violet::Input::IsKeyPressed(Violet::Key::A)) {
			m_cameraPosition.x -= m_cameraMovementSpeed *deltaTime;
		}
		else if (Violet::Input::IsKeyPressed(Violet::Key::D)) {
			m_cameraPosition.x += m_cameraMovementSpeed * deltaTime;
		}

		if (Violet::Input::IsKeyPressed(Violet::Key::W)) {
			m_cameraPosition.y += m_cameraMovementSpeed * deltaTime;
		}
		else if (Violet::Input::IsKeyPressed(Violet::Key::S)) {
			m_cameraPosition.y -= m_cameraMovementSpeed * deltaTime;
		}

		if (Violet::Input::IsKeyPressed(Violet::Key::E)) {
			m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
		}
		else if (Violet::Input::IsKeyPressed(Violet::Key::Q)) {
			m_cameraRotation += m_cameraRotationSpeed * deltaTime;
		}

		//Object controls
		if (Violet::Input::IsKeyPressed(Violet::Key::LEFT)) {
			m_objectPosition.x -= m_objectMovementSpeed * deltaTime;
		}
		else if (Violet::Input::IsKeyPressed(Violet::Key::RIGHT)) {
			m_objectPosition.x += m_objectMovementSpeed * deltaTime;
		}

		if (Violet::Input::IsKeyPressed(Violet::Key::UP)) {
			m_objectPosition.y += m_objectMovementSpeed * deltaTime;
		}
		else if (Violet::Input::IsKeyPressed(Violet::Key::DOWN)) {
			m_objectPosition.y -= m_objectMovementSpeed * deltaTime;
		}

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotationZ(m_cameraRotation);

		Violet::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Violet::RenderCommand::Clear();

		Violet::Renderer::BeginScene(m_camera);

		m_texture->bind(0);
		//m_shader->setFloat4("u_color", m_objectColor); //Note: will not be set up at the first frame,cuz the shader is not bound, but it will be bound when submit is called
		Violet::Renderer::Submit(m_shader, m_vertexArray, glm::translate(glm::mat4(1.0f),m_objectPosition));

		m_transparentTexture->bind(0);
		Violet::Renderer::Submit(m_shader, m_vertexArray, glm::translate(glm::mat4(1.0f), m_objectPosition));

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

	void onImGuiRender() override {
		ImGui::Begin("Object Color");
		ImGui::ColorEdit4("Object Color", glm::value_ptr(m_objectColor));
		ImGui::End();
	
	}

private:
	Violet::Ref<Violet::Shader> m_shader;
	Violet::Ref<Violet::Texture2D> m_texture, m_transparentTexture;
	Violet::Ref<Violet::VertexArray> m_vertexArray;

	Violet::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraRotation;
	float m_cameraMovementSpeed = 0.3f;
	float m_cameraRotationSpeed = 90.0f;
	
	glm::vec3 m_objectPosition;
	float m_objectMovementSpeed = 0.5f;
	glm::vec4 m_objectColor = {1.0f, 0.7f, 0.6f, 1.0f};
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