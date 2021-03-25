#include "GameLayer2D.h"

GameLayer2D::GameLayer2D() : Layer("2D Layer"), m_cameraController(1280.0f / 720.0f, true)
{
}

void GameLayer2D::onAttach()
{
	m_LetterVTexture = Violet::Texture2D::Create("assets/textures/LetterV_RGBA.png");
	m_transparentTexture = Violet::Texture2D::Create("assets/textures/Checkerboard_RGB.png");
}

void GameLayer2D::onDetach()
{
}

void GameLayer2D::onUpdate(Violet::DeltaTime& deltaTime)
{
	//Update
	m_cameraController.onUpdate(deltaTime);

	/*Animation*/
	m_objectPosition.x = m_objectPosition.x > m_cameraController.getRight() ? m_objectPosition.x = m_cameraController.getLeft() : m_objectPosition.x + 0.009f;
	m_objectRotation = m_objectRotation == 360.0f ? 0.0f : m_objectRotation + 1.0f;

	//Render m
	Violet::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Violet::RenderCommand::Clear();

	//TODO: Should call Renderer2D::Shutdown() in Renderer's Shutdown()-> "To be made function". 
	Violet::Renderer2D::BeginScene(m_cameraController.getCamera());
	
	Violet::Renderer2D::DrawQuad(m_objectPosition, { 0.2f, 0.5f }, m_objectColor);
	Violet::Renderer2D::DrawQuad({ -1.5f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
	Violet::Renderer2D::DrawQuad({ -1.2f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
	Violet::Renderer2D::DrawQuad({ -0.9f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
	Violet::Renderer2D::DrawQuad({ -0.6f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
	Violet::Renderer2D::DrawQuad({ -0.3f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
	//Violet::Renderer2D::DrawRotatedQuad({ 0.6f, 0.2f }, { 0.5f, 0.5f }, -m_objectRotation, {1.0f, 0.93f, 0.24f, 1.0f});
	//Violet::Renderer2D::DrawRotatedQuad({ -0.6f, 0.2f }, { 0.5f, 0.5f },  m_objectRotation, { 0.18f, 0.6f, 0.96f, 1.0f});
	//Background
	Violet::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { m_cameraController.getWidth() ,  m_cameraController.getHeight() }, m_transparentTexture, 2.0f);
	Violet::Renderer2D::DrawRotatedQuad({  0.5f, -0.5f }, { 0.5f, 0.5f }, -45.0f, m_LetterVTexture);
	Violet::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, 45.0f, { 0.18f, 0.6f, 0.96f, 1.0f });

	Violet::Renderer2D::EndScene();
}

void GameLayer2D::onImGuiRender()
{
	ImGui::Begin("Render2D Scene Statistics");
	ImGui::Text("Draw Calls: %d", Violet::Renderer2D::GetSceneStatistics().getTotalDrawCallsCount());
	ImGui::Text("Quads: %d", Violet::Renderer2D::GetSceneStatistics().getTotalQuadCount());
	ImGui::Text("Vertices: %d", Violet::Renderer2D::GetSceneStatistics().getTotalVertexCount());
	ImGui::Text("Indices: % d", Violet::Renderer2D::GetSceneStatistics().getTotalIndexCount());
	ImGui::End();

	ImGui::Begin("Object Color");	
	ImGui::ColorEdit4("Object Color", glm::value_ptr(m_objectColor));
	ImGui::End();

	ImGui::Begin("FPS");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void GameLayer2D::onEvent(Violet::Event& e)
{
	m_cameraController.onEvent(e);

}
