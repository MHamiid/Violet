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
			m_cameraController.onResize(m_viewPortSize.x, m_viewPortSize.y);
		}
		if (m_viewPortFocused) //Update the camera movement only when the viewport is focused 
		{
			m_cameraController.onUpdate(deltaTime);
		}
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
		Renderer2D::DrawRotatedQuad({ m_cameraController.getCamera().getPosition().x, m_cameraController.getCamera().getPosition().y, -0.1f },
			{ m_cameraController.getWidth(), m_cameraController.getHeight() }, m_cameraController.getCamera().getRotationZ(), m_transparentTexture, 2.0f);
		Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, -45.0f, m_LetterVTexture);
		Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, 45.0f, m_grassTexture);

		Renderer2D::EndScene();

		m_frameBuffer->unBind();  //NOTE: Must unBind the frame buffer to render on the window screen outside the frame buffer and for ImGui to work

	}

	void EditorLayer::onImGuiRender()
	{
		/*Begining ImGui DockSpace Code*/
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Violet"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("VSync On")) Violet::Application::GetApplicationInstance().getWindow().setVSync(true);
				if (ImGui::MenuItem("VSync Off")) Violet::Application::GetApplicationInstance().getWindow().setVSync(false);
				if (ImGui::MenuItem("Exit")) Violet::Application::GetApplicationInstance().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		/*Ending ImGui DockSpace Code*/

		/*Start ImGui Code*/
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

		m_viewPortFocused = ImGui::IsWindowFocused();  //Get the ViewPort focus status from ImGui and update the status
		m_viewPortHovered = ImGui::IsWindowHovered();  //Get the ViewPort hover status from ImGui and update the status

		Application::GetApplicationInstance().getImGuiLayer()->setImGuiToBlockEvents(!(m_viewPortFocused && m_viewPortHovered));
	

		//Get the size of the panel
		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		m_viewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };  //Update the size
		
		ImGui::Image((void*)(m_frameBuffer->getColorAttachmentID()), ImVec2(viewPortPanelSize.x, viewPortPanelSize.y), ImVec2(0, 1), ImVec2(1, 0));  //Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default
		ImGui::End();
		ImGui::PopStyleVar();   //Restore the original padding for other ImGui panels
		/*End ImGui Code*/

		ImGui::End();  //DockSpace ImGui::End
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);

	}
}