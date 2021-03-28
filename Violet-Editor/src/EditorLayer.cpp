#include "EditorLayer.h"

namespace Violet {

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::onAttach()
	{
		m_LetterVTexture = Texture2D::Create("assets/textures/LetterV_RGBA.png");
		m_transparentTexture = Texture2D::Create("assets/textures/Checkerboard_RGB.png");
		m_grassTexture = Texture2D::Create("assets/textures/WildGrass_1024x1024.png");

		//Create specs for the frame buffer
		FrameBufferSpecification specs;
		specs.width = 1280; //Window width
		specs.height = 720; //Window height

		m_frameBuffer = FrameBuffer::Create(specs);
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(DeltaTime& deltaTime)
	{
		//Update

		//Update the frame buffer size and camera projection if the ViewPort Panel size has changed
		FrameBufferSpecification frameBufferSpec = m_frameBuffer->getSpecification();  //Get the current frame buffer size
		//Check if the current frame buffer size differs from the new ImGui ViewPort Panel
		if ((m_viewPortSize.x != frameBufferSpec.width || m_viewPortSize.y != frameBufferSpec.height) && (m_viewPortSize.x > 0.0f && m_viewPortSize.y > 0.0f))   //NOTE: Check that x, y are not zero, which results in Frame Buffer Incomplete Status
		{
			//Update the framebuffer and the viewport in FrameBuffer.onBind function
			m_frameBuffer->resize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);

			//Update the camera
			m_cameraController.onResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		}

		m_cameraController.onUpdate(deltaTime);

		/*Animation*/
		m_objectPosition.x = m_objectPosition.x > m_cameraController.getRight() ? m_objectPosition.x = m_cameraController.getLeft() : m_objectPosition.x + (0.3f * deltaTime);
		m_objectRotation = m_objectRotation == 360.0f ? 0.0f : m_objectRotation + (30.0f * deltaTime);

		//Render

		//Clears the window screen
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();

		//Render FrameBuffer
		m_frameBuffer->bind();

		//Clears the frame buffer screen
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();


		Renderer2D::BeginScene(m_cameraController.getCamera());

		Renderer2D::DrawQuad(m_objectPosition, { 0.2f, 0.5f }, m_objectColor);
		Renderer2D::DrawQuad({ -1.5f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		Renderer2D::DrawQuad({ -1.2f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		Renderer2D::DrawQuad({ -0.9f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		Renderer2D::DrawQuad({ -0.6f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		Renderer2D::DrawQuad({ -0.3f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		Renderer2D::DrawRotatedQuad({ 0.6f, 0.2f }, { 0.5f, 0.5f }, -m_objectRotation, { 1.0f, 0.93f, 0.24f, 1.0f });
		Renderer2D::DrawRotatedQuad({ -0.6f, 0.2f }, { 0.5f, 0.5f }, m_objectRotation, { 0.18f, 0.6f, 0.96f, 1.0f });
		//Background
		Renderer2D::DrawRotatedQuad({ m_cameraController.getCamera().getPosition().x, m_cameraController.getCamera().getPosition().y, -0.1f }, { m_cameraController.getWidth() ,  m_cameraController.getHeight() }, m_cameraController.getCamera().getRotationZ(), m_transparentTexture, 2.0f);
		Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, -45.0f, m_LetterVTexture);
		Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, 45.0f, m_grassTexture);

		Renderer2D::EndScene();

		m_frameBuffer->unBind();  //NOTE: Must unBind the frame buffer to render on the window screen outside the frame buffer and for ImGui to work

	}

	void EditorLayer::onImGuiRender()
	{
		ImGui::Begin("Render2D Scene Statistics");
		ImGui::Text("Draw Calls: %d", Renderer2D::GetSceneStatistics().getTotalDrawCallsCount());
		ImGui::Text("Quads: %d", Renderer2D::GetSceneStatistics().getTotalQuadCount());
		ImGui::Text("Vertices: %d", Renderer2D::GetSceneStatistics().getTotalVertexCount());
		ImGui::Text("Indices: %d", Renderer2D::GetSceneStatistics().getTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Object Color");
		ImGui::ColorEdit4("Object Color", glm::value_ptr(m_objectColor));
		ImGui::End();

		ImGui::Begin("FPS");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));  //Remove window padding for the viewport
		ImGui::Begin("ViewPort");

		//Get the size of the panel
		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		m_viewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };  //Update the size
		
		ImGui::Image((void*)(m_frameBuffer->getColorAttachmentID()), ImVec2(viewPortPanelSize.x, viewPortPanelSize.y), ImVec2(0, 1), ImVec2(1, 0));  //Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default
		ImGui::End();
		ImGui::PopStyleVar();   //Restore the original padding for other ImGui panels
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);

	}
}