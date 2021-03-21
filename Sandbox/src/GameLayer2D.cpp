#include "GameLayer2D.h"

GameLayer2D::GameLayer2D() : Layer("2D Layer"), m_cameraController(1280.0f / 720.0f, true)
{
}

void GameLayer2D::onAttach()
{
	m_transparentTexture = Violet::Texture2D::Create("assets/textures/Checkerboard_RGB.png");
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

	//TODO: Should call Renderer2D::Shutdown() in Renderer's Shutdown()-> "To be made function". 
	Violet::Renderer2D::BeginScene(m_cameraController.getCamera());

	Violet::Renderer2D::DrawQuad({ -0.9f, 0.0f }, { 0.2f, 0.5f },  30.0f, m_objectColor);
	Violet::Renderer2D::DrawQuad({  0.6f, 0.2f }, { 0.5f, 0.5f }, -30.0f, m_objectColor);
	Violet::Renderer2D::DrawQuad({  0.0f,  0.0f, -0.1f }, { m_cameraController.getWidth() ,  m_cameraController.getHeight() },  0.0f, m_transparentTexture);

	Violet::Renderer2D::EndScene();
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
