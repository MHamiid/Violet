#include "EditorLayer.h"
#include "Violet/Scene/SceneSerializer.h"
#include "Violet/Utils/PlatformUtils.h"
#include <ImGuizmo.h>
#include "Violet/Math/Math.h"
#include <misc/cpp/imgui_stdlib.h>

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

		/*Scene And Context Initialization*/
		m_activeScene = CreateRef<Scene>("Untitled Scene");
		m_sceneHierarchyPanel.setSceneContext(m_activeScene);
		m_sceneHierarchyPanel.setPropertiesPanelContext(&m_propertiesPanel);

		//Initialize Gizmos to be in translation mode
		m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;

		class CameraController : public Script {
		public:
			float m_translationSpeed = 5.0f;
			float m_rotationSpeed = 3.0f;

			glm::vec3* m_translation = nullptr;
			glm::vec3* m_rotation = nullptr;

			void onCreate() override 
			{
				VIO_CORE_INFO("[Script] CameraController::onCreate");
				m_translation = &getComponent<TransformComponent>().translation;
				m_rotation = &getComponent<TransformComponent>().rotation;
			}
			void onUpdate(DeltaTime deltaTime) override
			{

				/*Camera Controls*/
				if (Input::IsKeyPressed(Violet::Key::A)) {
					m_translation->x -= m_translationSpeed * deltaTime;
				}
				else if (Input::IsKeyPressed(Violet::Key::D)) {
					m_translation->x += m_translationSpeed * deltaTime;
				}

				if (Input::IsKeyPressed(Violet::Key::W)) {
					m_translation->y += m_translationSpeed * deltaTime;
				}
				else if (Input::IsKeyPressed(Violet::Key::S)) {
					m_translation->y -= m_translationSpeed * deltaTime;
				}

				//Rotation
				if (Input::IsKeyPressed(Violet::Key::E)) {
					m_rotation->z -= m_rotationSpeed * deltaTime;
				}
				else if (Input::IsKeyPressed(Violet::Key::Q)) {
					m_rotation->z += m_rotationSpeed * deltaTime;
				}

				
			}
			void onDestroy() override
			{
				//TODO
				//Not callable yet
			}
		};

	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(DeltaTime& deltaTime)
	{
		/*Update*/

		//Update the frame buffer size and camera projection if the ViewPort Panel size has changed
		FrameBufferSpecification frameBufferSpec = m_frameBuffer->getSpecification();  //Get the current frame buffer size
		//Check if the current frame buffer size differs from the new ImGui ViewPort Panel
		if ((m_viewPortSize.x != frameBufferSpec.width || m_viewPortSize.y != frameBufferSpec.height) && (m_viewPortSize.x > 0.0f && m_viewPortSize.y > 0.0f))   //NOTE: Check that x, y are not zero, which results in Frame Buffer Incomplete Status
		{
			//Update the framebuffer and the viewport in FrameBuffer.onBind function
			m_frameBuffer->resize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);

			//Update the camera
			//m_cameraController.onResize(m_viewPortSize.x, m_viewPortSize.y);

			m_activeScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		}
		if (m_viewPortFocused) //Update the camera movement only when the viewport is focused 
		{
			//m_cameraController.onUpdate(deltaTime);
		}


		/*Animation*/
		//m_translationSpeed = m_objectPosition.x >= m_cameraController.getRight() || m_objectPosition.x <= m_cameraController.getLeft() ? -1.0f * m_translationSpeed : m_translationSpeed;
		//m_objectPosition.x +=(m_translationSpeed * deltaTime);
		//m_objectRotationZ = m_objectRotationZ >= 360.0f ? m_objectRotationZ - 360.0f : m_objectRotationZ + (30.0f * deltaTime);  //(m_objectRotationZ - 360.0f) resets the rotation counter instead of setting it to 0.0f, cause m_objectRotationZ may have exceeded 360.0f
		//m_scaleSpeed = m_objectScale.x >= 1.0f || m_objectScale.x <= 0.1f ? -1.0f * m_scaleSpeed : m_scaleSpeed;
		//m_objectScale += (m_scaleSpeed * deltaTime);
		/*Render*/

		//Clears the window screen
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();

		//Render FrameBuffer
		m_frameBuffer->bind();

		//Clears the frame buffer screen
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();


		//Renderer2D::BeginScene(m_cameraController.getCamera());

		//Update/Render the scene
		m_activeScene->onUpdate(deltaTime);
		/*TransformComponent& squareTransformation = m_squareEntity.getComponent<TransformComponent>();
		squareTransformation.translation = m_objectPosition;
		squareTransformation.rotation = { 0.0f, 0.0f, m_objectRotationZ };
		squareTransformation.scale = m_objectScale;*/

		//Renderer2D::DrawQuad(m_objectPosition, { 0.2f, 0.5f }, m_objectColor);
		//Renderer2D::DrawQuad({ -1.5f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		//Renderer2D::DrawQuad({ -1.2f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		//Renderer2D::DrawQuad({ -0.9f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		//Renderer2D::DrawQuad({ -0.6f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		//Renderer2D::DrawQuad({ -0.3f, 0.0f }, { 0.2f, 0.2f }, m_objectColor);
		//Renderer2D::DrawRotatedQuad({ 0.6f, 0.2f }, { 0.5f, 0.5f }, -m_objectRotation, { 1.0f, 0.93f, 0.24f, 1.0f });
		//Renderer2D::DrawRotatedQuad({ -0.6f, 0.2f }, { 0.5f, 0.5f }, m_objectRotation, { 0.18f, 0.6f, 0.96f, 1.0f });
		////Background
		//Renderer2D::DrawRotatedQuad({ m_cameraController.getCamera().getPosition().x, m_cameraController.getCamera().getPosition().y, -0.1f },
		//	{ m_cameraController.getWidth(), m_cameraController.getHeight() }, m_cameraController.getCamera().getRotationZ(), m_transparentTexture, 2.0f);
		//Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, -45.0f, m_LetterVTexture);
		//Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, 45.0f, m_grassTexture);

		//Renderer2D::EndScene();

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

		/*Set Minimum Docked Window Size*/
		ImGuiStyle& style = ImGui::GetStyle();
		float originalMinWindowSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;


		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		//Set back the original value
		style.WindowMinSize.x = originalMinWindowSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("New", "Ctrl+N")) 
				{
					m_showNewScenePopupModal = true;
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					openScene();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					if (!m_activeScenePath.empty())
					{
						saveScene();
					}
					else
					{
						saveSceneAs();
					}
				}
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					saveSceneAs();
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					Application::GetApplicationInstance().close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();

		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Violet"))
			{
				if (ImGui::MenuItem("VSync On")) Application::GetApplicationInstance().getWindow().setVSync(true);
				if (ImGui::MenuItem("VSync Off")) Application::GetApplicationInstance().getWindow().setVSync(false);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}



		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Theme"))
			{
				if (ImGui::MenuItem("Violet")) Application::GetApplicationInstance().getImGuiLayer()->SetVioletThemeColors();
				if (ImGui::MenuItem("Gold")) Application::GetApplicationInstance().getImGuiLayer()->SetGoldThemeColors();
				if (ImGui::MenuItem("Dark")) Application::GetApplicationInstance().getImGuiLayer()->SetDarkThemeColors();
				if (ImGui::MenuItem("Dark Red")) Application::GetApplicationInstance().getImGuiLayer()->SetDarkRedThemeColors();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		/*Ending ImGui DockSpace Code*/


		/*Start ImGui Code*/
		m_sceneHierarchyPanel.onImGuiRender();
		m_propertiesPanel.onImGuiRender();

		ImGui::Begin("Renderer2D Scene Statistics");
		ImGui::Text("Draw Calls: %d", Renderer2D::GetSceneStatistics().getTotalDrawCallsCount());
		ImGui::Text("Quads: %d", Renderer2D::GetSceneStatistics().getTotalQuadCount());
		ImGui::Text("Vertices: %d", Renderer2D::GetSceneStatistics().getTotalVertexCount());
		ImGui::Text("Indices: %d", Renderer2D::GetSceneStatistics().getTotalIndexCount());
		ImGui::End();

		ImGui::Begin("FPS");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));  //Remove window padding for the viewport
		ImGui::Begin("ViewPort");

		m_viewPortFocused = ImGui::IsWindowFocused();  //Get the ViewPort focus status from ImGui and update the status
		m_viewPortHovered = ImGui::IsWindowHovered();  //Get the ViewPort hover status from ImGui and update the status

		if (!ImGui::IsAnyItemActive())
		{
			Application::GetApplicationInstance().getImGuiLayer()->setImGuiToBlockEvents(!m_viewPortFocused && !m_viewPortHovered);
		}
		else
		{
			Application::GetApplicationInstance().getImGuiLayer()->setImGuiToBlockEvents(!(m_viewPortFocused && m_viewPortHovered));
		}

		//Get the size of the panel
		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		m_viewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };  //Update the size
		
		uint64_t textureID = m_frameBuffer->getColorAttachmentID();  //Change uint32_t to uint64_t to match with the 64 bit void* pointer when casting
		ImGui::Image((void*)textureID, ImVec2(viewPortPanelSize.x, viewPortPanelSize.y), ImVec2(0, 1), ImVec2(1, 0));  //Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default

		//TODO: Set UI buttons for selecting the gizmo type
		//ImGizmos
		Entity selectedEntity =  m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity && m_gizmoType!= -1 && m_activeScene->getPrimaryCameraEntity()) //If the selected entity and the primary camera entity are valid entities and a we have a gizmo to use
		{
			ImGuizmo::SetDrawlist();  //Draw to the current window
			
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_viewPortSize.x, m_viewPortSize.y);  //Set the view port

			/*Get Camera Info From The Primary Camera*/
			Entity primaryCameraEntity = m_activeScene->getPrimaryCameraEntity();
			const SceneCamera& primaryCamera = primaryCameraEntity.getComponent<CameraComponent>().sceneCamera;

			const glm::mat4& primaryCameraProjectionMatrix = primaryCamera.getProjectionMatrix();
			glm::mat4 primaryCameraViewMatrix = glm::inverse(primaryCameraEntity.getComponent<TransformComponent>().getTransform());


			/*Get The Selected Entity Info*/
			TransformComponent& selectedEntityTransformComponent = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 selectedEntityGizmosTransform = selectedEntityTransformComponent.getTransform();
	
			/*Get The Correct Snapping Value*/
			float snapValue = 0.0f;
			bool snapEnabled = Input::IsKeyPressed(Key::LEFT_CONTROL);
			//TODO: Create A UI For Setting m_snapValue
			if (snapEnabled)
			{
				if (m_gizmoType == ImGuizmo::OPERATION::TRANSLATE) 
				{
					snapValue = m_snapValues[0];
				}
				else if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
				{
					snapValue = m_snapValues[1];
				}
				else if (m_gizmoType == ImGuizmo::OPERATION::SCALE)
				{
					snapValue = m_snapValues[2];
				}
			}
			float snapValuesAxes[3] = { snapValue, snapValue, snapValue };


			//Pass the parameters to ImGuizmo to draw the gizmos and write the new transformation to the selectedEntityGizmosTransform matrix
			ImGuizmo::Manipulate(glm::value_ptr(primaryCameraViewMatrix), glm::value_ptr(primaryCameraProjectionMatrix)
			, (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedEntityGizmosTransform)
			, nullptr, snapEnabled ? snapValuesAxes : nullptr);

			if (ImGuizmo::IsUsing()) //Return true if mouse IsOver or if the gizmo is in moving state
			{	
				/*Apply the gizmo transformation changes to the selected entity*/

				glm::vec3 selectedEntityTranslation, selectedEntityRotation, selectedEntityScale;  //Note that these are not initialized

				//Get the translation, rotation, scale from the transformation matrix
				Math::decomposeTransformationMatrix(selectedEntityGizmosTransform, selectedEntityTranslation, selectedEntityRotation, selectedEntityScale);

				//Set the transformations
				selectedEntityTransformComponent.translation = selectedEntityTranslation;
				selectedEntityTransformComponent.rotation = selectedEntityRotation;
				selectedEntityTransformComponent.scale = selectedEntityScale;
			
			}

		}
		ImGui::End();  //ViewPort
	
		/*New Scene Popup Modal Dialogue*/
		if (m_showNewScenePopupModal) {
			ImGui::OpenPopup("New Scene"); //This name(ID) should be the same as BeginPopupModal

		    // Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("##NewSceneName-ID", &m_newSceneNameBuffer);
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					m_showNewScenePopupModal = false;
					newScene(m_newSceneNameBuffer);
					ImGui::CloseCurrentPopup();
				}

				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					m_showNewScenePopupModal = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::PopStyleVar();   //Restore the original padding for other ImGui panels
		/*End ImGui Code*/

		ImGui::End();  //DockSpace ImGui::End
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(VIO_BIND_EVENT_FUNCTION(EditorLayer::onKeyPressed));
		dispatcher.dispatch<ItemsDroppedEvent>(VIO_BIND_EVENT_FUNCTION(EditorLayer::onItemsDropped));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& event)
	{
		/*Shortcuts*/
		/*Note Currently ImGui Is Set To Block The Events Unless IF The ViewPort Is Fucosed Or Hovered*/
		if (event.getRepeatCount() > 0) 
		{
			return false;
		}

		bool controlKeyIsPressed = Input::IsKeyPressed(Key::LEFT_CONTROL) || Input::IsKeyPressed(Key::RIGHT_CONTROL);
		bool shiftKeyIsPressed = Input::IsKeyPressed(Key::LEFT_SHIFT) || Input::IsKeyPressed(Key::RIGHT_SHIFT);

		switch (event.getKeyCode())
		{
		/*File Menu*/
		case Key::N:
		{
			if (controlKeyIsPressed) 
			{
				m_showNewScenePopupModal = true;
			}
			return true;
		}
		case Key::O:
		{
			if (controlKeyIsPressed) 
			{
				openScene();
			}
			return true;
		}
		case Key::S:
		{
			if (controlKeyIsPressed && shiftKeyIsPressed) 
			{
				saveSceneAs();
			}
			if (controlKeyIsPressed)
			{
				if (!m_activeScenePath.empty())
				{
					saveScene();
				}
				else
				{
					saveSceneAs();
				}
			}
			/*Gizmos*/
			if (!ImGuizmo::IsUsing() && m_sceneHierarchyPanel.getSelectedEntity()) { m_gizmoType = ImGuizmo::OPERATION::SCALE; }
			return true;
		}
		/*Gizmos*/
		case Key::ESCAPE:
			m_gizmoType = -1;
			return true;

		case Key::G:
			if (!ImGuizmo::IsUsing() && m_sceneHierarchyPanel.getSelectedEntity()) { m_gizmoType = ImGuizmo::OPERATION::TRANSLATE; }
			return true;

		case Key::R:
			if (!ImGuizmo::IsUsing() && m_sceneHierarchyPanel.getSelectedEntity()) { m_gizmoType = ImGuizmo::OPERATION::ROTATE; }
			return true;

		default:
			return false;
		}
	}
	bool EditorLayer::onItemsDropped(ItemsDroppedEvent& event)
	{
		if (event.getItemsCount() > 1) 
		{
			VIO_CORE_ERROR("More Than One Scene Dropped!");
		}
		else
		{
			openScene(event.getItemsPaths()[0]);
		}


		return true;
	}
	void EditorLayer::newScene(const std::string& sceneName)
	{
		m_activeScene = CreateRef<Scene>(sceneName); //Reset the current active scene
		m_activeScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		m_sceneHierarchyPanel.setSceneContext(m_activeScene);
		
	}
	void EditorLayer::openScene()
	{
		std::optional<std::string> filePath = FileDialogs::OpenFile("Violet Scene (*.violet)\0*.violet\0");

		openScene(*filePath);
	}
	void EditorLayer::openScene(const std::string& filePath)
	{
		//TODO: Check the extension of the file
		if (!filePath.empty())  //If the string is not empty 
		{
			m_activeScene = CreateRef<Scene>(); //Reset the current active scene
			m_activeScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
			m_sceneHierarchyPanel.setSceneContext(m_activeScene);

			SceneSerializer sceneSerializer(m_activeScene);
			if (sceneSerializer.deserializeText(filePath))
			{
				m_activeScenePath = filePath;
			}
			else 
			{
				VIO_CORE_ERROR("Couldn't Open '{0}'", filePath);
			}
		}
	}
	void EditorLayer::saveScene()
	{
		//Extra check that the m_activeScenePath is not empty, this function shouldn't be called anyway if the m_activeScenePath is empty
		if (!m_activeScenePath.empty())
		{
			SceneSerializer sceneSerializer(m_activeScene);
			sceneSerializer.serializeToText(m_activeScenePath);
		}
		else 
		{
			VIO_CORE_ERROR("Attempting To Save With An Empty Scene Name!");
		}

	}
	void EditorLayer::saveSceneAs()
	{
		std::optional<std::string> filePath = FileDialogs::SaveFile("Violet Scene (*.violet)\0*.violet\0", std::string(m_activeScene->getSceneName() + ".violet").c_str());

		if (filePath)  //If the string is not empty 
		{
			SceneSerializer sceneSerializer(m_activeScene);
			sceneSerializer.serializeToText(*filePath);

			m_activeScenePath = *filePath;
		}
	}
}