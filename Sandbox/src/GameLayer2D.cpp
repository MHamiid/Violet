#include "GameLayer2D.h"

GameLayer2D::GameLayer2D() : Layer("2D Layer"), m_cameraController(1280.0f / 720.0f, true)
{
}

void GameLayer2D::onAttach()
{
	m_vertexArray = Violet::VertexArray::Create();



	float vertices[] = {
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	   -0.5f,  0.5f, 0.0f
	};


	Violet::Ref<Violet::VertexBuffer> vertexBuffer = Violet::VertexBuffer::Create(vertices, sizeof(vertices));


	Violet::VertexLayout layout = { {Violet::VertexAttributeDataType::Float3, "Position"}};

	vertexBuffer->setLayout(layout);

	m_vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	Violet::Ref<Violet::IndexBuffer> indexBuffer = Violet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_vertexArray->setIndexBuffer(indexBuffer);


	m_shader = Violet::Shader::Create("assets/shaders/SolidColor.glsl");
}

void GameLayer2D::onDetach()
{
}

void GameLayer2D::onUpdate(Violet::DeltaTime& deltaTime)
{
	//Update
	m_cameraController.onUpdate(deltaTime);

	//Render
	Violet::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Violet::RenderCommand::Clear();

	Violet::Renderer::BeginScene(m_cameraController.getCamera());

	
	m_shader->setFloat4("u_color", m_objectColor); //Note: will not be set up at the first frame,cuz the shader is not bound, but it will be bound when submit is called
	Violet::Renderer::Submit(m_shader, m_vertexArray, glm::translate(glm::mat4(1.0f), m_objectPosition));

	Violet::Renderer::EndScene();
}

void GameLayer2D::onImGuiRender()
{
	ImGui::Begin("Object Color");
	ImGui::ColorEdit4("Object Color", glm::value_ptr(m_objectColor));
	ImGui::End();

}

void GameLayer2D::onEvent(Violet::Event& e)
{
	m_cameraController.onEvent(e);

}
