#include "EditorLayer.h"
#include "Violet/Scene/SceneSerializer.h"
#include "Violet/Utils/PlatformUtils.h"
#include <ImGuizmo.h>
#include "Violet/Math/Math.h"
#include <misc/cpp/imgui_stdlib.h>
#include <filesystem>
#include <imgui_internal.h>
#include "Violet/ImGui/ImGuiUtils.h"

namespace Violet {

	//TODO: Note that the assets path is hardcoded, extract the assets path from the Project (Project class not created yet).
	//g_assetsPath declaration
	extern const std::filesystem::path g_assetsPath; // [HARD-CODED][TEMP][GLOBAL]

	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{
	}

	void EditorLayer::onAttach()
	{
		/*Editor Resources*/
		/*Toolbar Textures*/
		m_iconPlay = Texture2D::Create("resources/icons/playButton.png");
		m_iconStop = Texture2D::Create("resources/icons/stopButton.png");

		/*Test Textures*/
		m_LetterVTexture = Texture2D::Create("assets/textures/LetterV_RGBA.png");
		m_transparentTexture = Texture2D::Create("assets/textures/Checkerboard_RGB.png");
		m_grassTexture = Texture2D::Create("assets/textures/WildGrass_1024x1024.png");

		//Create specs for the frame buffer
		FrameBufferSpecification specs;
		specs.textureAttachmentsSpecification = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH };
		/*TODO: Should be automated*/
		specs.width = 1280; //Window width
		specs.height = 720; //Window height

		m_frameBuffer = FrameBuffer::Create(specs);

		/*Scene And Context Initialization*/
		//Initialize a default scene
		m_editorScene = CreateRef<Scene>("Untitled Scene");
		/*Add a default quad*/
		Entity quadEntity = m_editorScene->createEntity("Quad");
		quadEntity.addComponent<SpriteRendererComponent>();

		m_sceneHierarchyPanel.setSceneContext(m_editorScene);
		m_sceneHierarchyPanel.setPropertiesPanelContext(&m_propertiesPanel);

		/*Editor Camera Initialization*/
		m_editorCamera = EditorCamera(45.0f, 0.0f, 0.1f, 1000.0f);

		//Initialize Gizmos to be in translation mode
		m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;

		//class CameraController : public Script {
		//public:
		//	float m_translationSpeed = 5.0f;
		//	float m_rotationSpeed = 3.0f;

		//	glm::vec3* m_translation = nullptr;
		//	glm::vec3* m_rotation = nullptr;

		//	void onCreate() override 
		//	{
		//		VIO_CORE_INFO("[Script] CameraController::onCreate");
		//		m_translation = &getComponent<TransformComponent>().translation;
		//		m_rotation = &getComponent<TransformComponent>().rotation;
		//	}
		//	void onUpdate(DeltaTime deltaTime) override
		//	{

		//		/*Camera Controls*/
		//		if (Input::IsKeyPressed(Violet::Key::A)) {
		//			m_translation->x -= m_translationSpeed * deltaTime;
		//		}
		//		else if (Input::IsKeyPressed(Violet::Key::D)) {
		//			m_translation->x += m_translationSpeed * deltaTime;
		//		}

		//		if (Input::IsKeyPressed(Violet::Key::W)) {
		//			m_translation->y += m_translationSpeed * deltaTime;
		//		}
		//		else if (Input::IsKeyPressed(Violet::Key::S)) {
		//			m_translation->y -= m_translationSpeed * deltaTime;
		//		}

		//		//Rotation
		//		if (Input::IsKeyPressed(Violet::Key::E)) {
		//			m_rotation->z -= m_rotationSpeed * deltaTime;
		//		}
		//		else if (Input::IsKeyPressed(Violet::Key::Q)) {
		//			m_rotation->z += m_rotationSpeed * deltaTime;
		//		}

		//		
		//	}
		//	void onDestroy() override
		//	{
		//		//TODO
		//		//Not callable yet
		//	}
		//};

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

			//Update the editor camera viewport
			m_editorCamera.setViewPortSize(m_viewPortSize.x, m_viewPortSize.y);

			m_editorScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);  //Always update the original scene
			if (m_runtimeScene)  //SceneState == SceneState::Play
			{
				m_runtimeScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y); //Updates the copied/temp scene (runtimeScene)
			}
		}
		//if (m_viewPortFocused)
		//{
		//}


		/*Animation*/
		//m_translationSpeed = m_objectPosition.x >= m_cameraController.getRight() || m_objectPosition.x <= m_cameraController.getLeft() ? -1.0f * m_translationSpeed : m_translationSpeed;
		//m_objectPosition.x +=(m_translationSpeed * deltaTime);
		//m_objectRotationZ = m_objectRotationZ >= 360.0f ? m_objectRotationZ - 360.0f : m_objectRotationZ + (30.0f * deltaTime);  //(m_objectRotationZ - 360.0f) resets the rotation counter instead of setting it to 0.0f, cause m_objectRotationZ may have exceeded 360.0f
		//m_scaleSpeed = m_objectScale.x >= 1.0f || m_objectScale.x <= 0.1f ? -1.0f * m_scaleSpeed : m_scaleSpeed;
		//m_objectScale += (m_scaleSpeed * deltaTime);
		/*Render*/

		/*Clear the window screen, NOTE: No need to clear when using ImGui Dock Space*/
		//RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		//RenderCommand::Clear();

		//Render FrameBuffer
		m_frameBuffer->bind();

		/*Clear all of the framebuffer attachments*/
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();

		/*Clear the attachments*/
	
		/*Clear the entityID attachment to -1 ====> no entity*/
		int clearValueInt = -1;
		m_frameBuffer->clearColorAttachment(1, &clearValueInt);

		/*Render/Update The Scene*/
		//NOTE: Rendering must be here after the framebuffer is bound and cleared
		switch (m_sceneState) 
		{
			case SceneState::Edit:
			{
				//Update the editor camera movement
				if (m_viewPortFocused || m_viewPortHovered)
				{
					m_editorCamera.onUpdate(deltaTime);
				}

				//Render the scene
				m_editorScene->onUpdateEditor(deltaTime, m_editorCamera);
				break;
			}
			case SceneState::Play:
			{
				//Render the scene
				m_runtimeScene->onUpdateRuntime(deltaTime);
				break;
			}
		}
		onOverlayRender();

		/*Entity Mouse Selection*/
		if (m_updateMouseSelectedEntityID)
		{
			//Global mouse position
			ImVec2 mousePosition = ImGui::GetMousePos();
			/*Set the top left to (0, 0)*/
			mousePosition.x -= m_viewPortBounds[0].x;  //  m_viewPortBounds[0].x ====> Min bound X
			mousePosition.y -= m_viewPortBounds[0].y;  //  m_viewPortBounds[0].y ====> Min bound Y

			//Flip the coordinates to match with ImGui texture layout, set the bottom left to be (0, 0)
			mousePosition.y = m_viewPortSize.y - mousePosition.y;

			/*If the mouse is in the view port bounds*/
			if (mousePosition.x >= 0 && mousePosition.y >= 0 && mousePosition.x < m_viewPortSize.x && mousePosition.y < m_viewPortSize.y)
			{
				/*Read a pixel at the mouse coordinates from the FrameBuffer's color attachment (1) that has entity IDs stored at*/
				int pixelValue;
				//Must be called before m_frameBuffer->unBind()
				m_frameBuffer->readColorAttachmentPixel(1, (int)mousePosition.x, (int)mousePosition.y, &pixelValue);
	
				if (pixelValue == -1)  //If not valid entity, no entities in that pixel
				{
					m_sceneHierarchyPanel.setSelectedEntity({}); //Set a non-valid entity
				}
				else 
				{
					m_sceneHierarchyPanel.setSelectedEntity({ (entt::entity)pixelValue, m_editorScene.get() });
				}
			}
			m_updateMouseSelectedEntityID = false;
		}


		m_frameBuffer->unBind();  //NOTE: Must unBind the frame buffer to render on the window screen outside the frame buffer and for ImGui to work

	}

	void EditorLayer::onOverlayRender()
	{
		/*Get The Active Scene And Setup The Correct Renderer2D Camera Configuration For the Scene Type*/
		Ref<Scene> activeScene;   //TODO: Find a better way, instead of creating a new reference of the scene everytime the onOverlayRender() is called
		if (m_sceneState == SceneState::Play)
		{
			activeScene = m_runtimeScene;
			Entity cameraEntity = m_runtimeScene->getPrimaryCameraEntity();
			Renderer2D::BeginScene(cameraEntity.getComponent<CameraComponent>().sceneCamera, cameraEntity.getComponent<TransformComponent>().getTransform());
		}
		else if (m_sceneState == SceneState::Edit)
		{
			activeScene = m_editorScene;
			m_editorScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
			Renderer2D::BeginScene(m_editorCamera);
		}
		else
		{
			VIO_WARN("Couldn't Select A Scene To Render Overlay");
		}

		/*Physics Collider Visualization*/

		/*Visualize Circle Colliders*/
		auto cc2dView = activeScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
		for (auto enttEntity : cc2dView)
		{
			auto [transformComponent, cc2dComponent] = cc2dView.get<TransformComponent, CircleCollider2DComponent>(enttEntity);

			/*Setting Up A Circle To Represent The Circle Collider*/
			/*
			* NOTE: Applying rotation does matter even for circles, as it is ulimately the collider's rotation around the original circle origin, otherwise when offsetting the collider from the actual circle position, it would give incorrect results
			*/
			glm::vec3 ccScale = transformComponent.scale * (cc2dComponent.Radius * 2.0f);   //Get the scale of the circle, and multiply with (the factor of the circle diameter multiplied by 2.0f to get the full diameter (size))

			glm::mat4 ccTransfrom = glm::translate(glm::mat4(1.0f), transformComponent.translation)  //Translate to the actual circle position
				* glm::rotate(glm::mat4(1.0f), transformComponent.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			/*
			* NOTE: The order where the collider offset is applied after the rotation around the circle original origin, and not the collider's origin with the offset applied.
			*       The order where the scale is applied after the offset is applied.
			*/
			ccTransfrom = glm::translate(ccTransfrom, glm::vec3(cc2dComponent.Offset, 0.001f))   //Add the collider's offset(X, Y), and Push the box collider a little bit forward on the Z-Axis to make it visible when rendered
			* glm::scale(glm::mat4(1.0f), ccScale);
			Renderer2D::DrawCircle(ccTransfrom, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.01f);
		}

		/*Visualize Box Colliders*/
		auto bc2dView = activeScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
		for (auto enttEntity : bc2dView)
		{
			auto [transformComponent, bc2dComponent] = bc2dView.get<TransformComponent, BoxCollider2DComponent>(enttEntity);

			/*Setting Up A Quad To Represent The Box Collider*/
			glm::vec3 bcScale = transformComponent.scale * glm::vec3(bc2dComponent.SizeFactor * 2.0f, 1.0f);   //Get the scale of the quad, and multiply with (the sizeFactor multiplied by 2.0f to get the full diameter (size))

			glm::mat4 bcTransfrom = glm::translate(glm::mat4(1.0f), transformComponent.translation)  //Translate to the actual quad position
				* glm::rotate(glm::mat4(1.0f), transformComponent.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			/*
			* NOTE: The order where the collider offset is applied after the rotation around the quad original origin, and not the collider's origin with the offset applied.
			*       The order where the scale is applied after the offset is applied.
			*/
			bcTransfrom = glm::translate(bcTransfrom, glm::vec3(bc2dComponent.Offset, 0.001f))   //Add the collider's offset(X, Y), and Push the box collider a little bit forward on the Z-Axis to make it visible when rendered
				* glm::scale(glm::mat4(1.0f), bcScale);
			Renderer2D::DrawRectangle(bcTransfrom, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}

		Renderer2D::EndScene();
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

		/*Ending ImGui DockSpace Code*/


		/*Start ImGui Code*/
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
					openSceneDialog();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					if (!m_editorScenePath.empty())
					{
						saveScene();
					}
					else
					{
						saveSceneAsDialog();
					}
				}
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					saveSceneAsDialog();
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

		
		//If the editor camera is in use, disable interaction to avoid pop-ups opening when the mouse is released inside the sceneHierarchyPanel
		m_sceneHierarchyPanel.onImGuiRender(m_editorCamera.isUsing());
		m_propertiesPanel.onImGuiRender();
		m_contentBrowserPanel.onImGuiRender();

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
		
		/*set the bounds of the view port panel*/
		ImVec2 viewportOffset = ImGui::GetCursorScreenPos();
		m_viewPortBounds[0] = { viewportOffset.x, viewportOffset.y };
		m_viewPortBounds[1] = { viewPortPanelSize.x + viewportOffset.x, viewPortPanelSize.y + viewportOffset.y };

		
		uint64_t textureID = m_frameBuffer->getColorAttachmentID();  //Change uint32_t to uint64_t to match with the 64 bit void pointer when casting
		//Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(viewPortPanelSize.x, viewPortPanelSize.y), ImVec2(0, 1), ImVec2(1, 0));


		/*Receive Dropped Payloads*/
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* contentBrowserPayload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"); //ContentBrowser payload
			if (contentBrowserPayload) //If not nullptr
			{
				const char* itemRelativePath = reinterpret_cast<const char*>(contentBrowserPayload->Data);

				//Open the dropped scene
				openScene((g_assetsPath / itemRelativePath).string());

				ImGui::EndDragDropTarget();
			}
		}


		//TODO: Set UI buttons for selecting the gizmo type
		//Rendering ImGizmos
		if (m_sceneState == SceneState::Edit)
		{
			Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
			/*If the selected entity is a valid entity which has a transform component attached to it and we have a gizmo to use
			, NOTE: check for  && m_runtimeScene->getPrimaryCameraEntity() if we are drawing gizmos with the runtime camera to check if the primary camera entity is valid*/
			if (selectedEntity && selectedEntity.hasComponent<TransformComponent>() && m_gizmoType != -1)
			{
				ImGuizmo::SetDrawlist();  //Draw to the current window

				ImGuizmo::SetRect(m_viewPortBounds[0].x, m_viewPortBounds[0].y, m_viewPortSize.x, m_viewPortSize.y);  //Set the view port bounds

				/*Runtime Camera Gizmos*/
				///*Get Camera Info From The Primary Camera*/
				//Entity primaryCameraEntity = m_runtimeScene->getPrimaryCameraEntity();
				//const SceneCamera& primaryCamera = primaryCameraEntity.getComponent<CameraComponent>().sceneCamera;

				//const glm::mat4& primaryCameraProjectionMatrix = primaryCamera.getProjectionMatrix();
				//glm::mat4 primaryCameraViewMatrix = glm::inverse(primaryCameraEntity.getComponent<TransformComponent>().getTransform());

				/*Editor Camera Gizmos*/
				const glm::mat4& editorCameraProjectionMatrix = m_editorCamera.getProjectionMatrix();
				glm::mat4 editorCameraViewMatrix = m_editorCamera.getViewMatrix();



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
				ImGuizmo::Manipulate(glm::value_ptr(editorCameraViewMatrix), glm::value_ptr(editorCameraProjectionMatrix)
					, (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedEntityGizmosTransform)
					, nullptr, snapEnabled ? snapValuesAxes : nullptr);

				if (ImGuizmo::IsUsing() && !m_editorCamera.isUsing()) //If mouse IsOver or if the gizmo is in moving state and the editor camera is not in use
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
				/*Set the ImGui::InputText to be auto-focused when the New Scene Popup Modal Dialogue is created if there is no item active (@ex: the Button is pressed)
				(we have the cursor in the inputText field by default, so that we can type upon opening the PopUpModal without the clicking the inputText field),
				otherwise the focus is set every frame, so it will steal the focus from the Button every frame and we can't click a button*/
				if (!ImGui::IsAnyItemActive())
					ImGui::SetKeyboardFocusHere(0);
				ImGui::InputText("##NewSceneName-ID", &m_newSceneNameBuffer);

				ImGui::Separator();
				Utils::ImGuiUtils::DrawWithHiddenStyle(m_newSceneNameBuffer.empty(), [&](bool itemHidden)
					{
						//If the button is [Left-Clicked] or the [Enter] key is pressed while the button is not hidden
						if (ImGui::Button("OK", ImVec2(120, 0)) || (!itemHidden && ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Enter])))
						{
							m_showNewScenePopupModal = false;
							newScene(m_newSceneNameBuffer);
							m_newSceneNameBuffer = ""; //Clear the buffer
							ImGui::CloseCurrentPopup();
						}
					});
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				//If the button is [Left-Clicked] or the [Escape] key is pressed
				if (ImGui::Button("Cancel", ImVec2(120, 0)) || ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Escape]))
				{
					m_showNewScenePopupModal = false;
					m_newSceneNameBuffer = ""; //Clear the buffer
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::PopStyleVar();   //Restore the original padding for other ImGui panels
		/*End ImGui Code*/

		UIToolbar();

		ImGui::End();  //DockSpace ImGui::End
	}
	void EditorLayer::UIToolbar()
	{
		/*Set Styles/Colors For The Window/Button*/
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0)); //Spacing between items
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		
		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);  //NOTE: Second parameter is set to nullptr, which means there is no close button for the window

		/*Play/Stop Button*/
		float buttonSize = ImGui::GetWindowHeight() - 4.0f;  //Gets the Toolbar height - Some padding value
		Ref<Texture2D> icon = m_sceneState == SceneState::Edit ? m_iconPlay : m_iconStop;
		uint64_t textureID = icon->getTextureID();  //Change uint32_t to uint64_t to match with the 64 bit void pointer ( ImTextureID = void* ) when casting
		//Centering the Button in middle of the Toolbar
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (buttonSize * 0.5f)); //NOTE: We can use ImGui::SameLine instead of ImGui::SetCursorPosX as a way to set a desired offset for the button (Propably would use it when we have more than one button in the toolbar)
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureID), ImVec2(buttonSize, buttonSize),
			ImVec2(0, 0), ImVec2(1, 1), 0)) //Remove frame padding
		{
			if (m_sceneState == SceneState::Edit)
			{
				onScenePlay();
			}
			else if (m_sceneState == SceneState::Play)
			{
				onSceneStop();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_editorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(VIO_BIND_EVENT_MEMBER_FUNCTION(EditorLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(VIO_BIND_EVENT_MEMBER_FUNCTION(EditorLayer::onMouseButtonPressed));
		dispatcher.dispatch<ItemsDroppedEvent>(VIO_BIND_EVENT_MEMBER_FUNCTION(EditorLayer::onItemsDropped));
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
				openSceneDialog();
			}
			return true;
		}
		case Key::S:
		{
			if (controlKeyIsPressed && shiftKeyIsPressed) 
			{
				saveSceneAsDialog();
				return true;
			}
			if (controlKeyIsPressed)
			{
				if (!m_editorScenePath.empty())
				{
					saveScene();
				}
				else
				{
					saveSceneAsDialog();
				}
				return true;
			}
			/*Gizmos*/
			if (!ImGuizmo::IsUsing() && m_sceneHierarchyPanel.getSelectedEntity()) { m_gizmoType = ImGuizmo::OPERATION::SCALE; }
			return true;
		}
		/*Scene Commands*/
		case Key::D:
		{
			if (controlKeyIsPressed)
			{
				onDuplicateEntity();
			}
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
	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		/*Entity Mouse Selection*/
		if (m_sceneState == SceneState::Edit && event.getMouseButton() == Mouse::BUTTON_LEFT && !m_editorCamera.isUsing() && m_viewPortHovered && !ImGuizmo::IsOver())
		{
			m_updateMouseSelectedEntityID = true;
		}
		return true;
	}
	/*Dropping Items From Outside The Editor*/
	bool EditorLayer::onItemsDropped(ItemsDroppedEvent& event)
	{
		if (event.getItemsCount() > 1) 
		{
			VIO_CORE_ERROR("More Than One Scene Dropped!");
		}
		else
		{
			std::filesystem::path filePath = event.getItemsPaths()[0];
			openScene(filePath.string());
		}


		return true;
	}
	void EditorLayer::newScene(const std::string& sceneName)
	{
		m_editorScene = CreateRef<Scene>(sceneName); //Reset the current editor scene
		m_editorScene->onViewPortResize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		m_sceneHierarchyPanel.setSceneContext(m_editorScene);
		m_editorScenePath = ""; //Reset the scene path to empty path (it's a new created scene, doesn't have a path, it's not read from a file)

		//Stop the runtime scene if running
		if (m_sceneState == SceneState::Play)
		{
			onSceneStop();
		}

		//Reset editor camera
		m_editorCamera = EditorCamera(45.0f, 0.0f, 0.1f, 1000.0f);
		//set the editor camera viewport
		m_editorCamera.setViewPortSize(m_viewPortSize.x, m_viewPortSize.y);
	}
	void EditorLayer::openSceneDialog()
	{
		std::string filePath = FileDialogs::OpenFile("Violet Scene (*.violet)\0*.violet\0");
		if (!filePath.empty())
		{
			openScene(filePath);
		}
	}
	void EditorLayer::openScene(const std::string& filePath)
	{
		//If there is a scene running stop it first before opening a new scene
		if (m_sceneState != SceneState::Edit)
		{
			onSceneStop();
		}

		if (std::filesystem::path(filePath).extension() == ".violet")
		{
			newScene(std::string()); //Pass the scene name as empty string, as that the SceneSerializer will deserialize the scene and get the scene name and set it
			if (deserializeScene(m_editorScene, filePath)) //If deserialization succeeded
			{
				m_editorScenePath = filePath;
			}
			else
			{
				VIO_CORE_ERROR("Couldn't Open '{0}'", filePath);
			}
		}
		else
		{
			VIO_CORE_ERROR("Attempting To Open An Invalid Scene File Type '{0}', try '.violet' File Type", filePath);
		}
		
		
	}
	void EditorLayer::saveScene()
	{
		//Extra check that the m_editorScenePath is not empty, this function shouldn't be called anyway if the m_editorScenePath is empty
		if (!m_editorScenePath.empty())
		{
			serializeScene(m_editorScene, m_editorScenePath);
		}
		else 
		{
			VIO_CORE_ERROR("Attempting To Save With An Empty Scene Name!");
		}

	}
	void EditorLayer::saveSceneAsDialog()
	{
		std::string filePath = FileDialogs::SaveFile("Violet Scene (*.violet)\0*.violet\0", std::string(m_editorScene->getSceneName() + ".violet").c_str());

		if (!filePath.empty())  //If the string is not empty 
		{
			serializeScene(m_editorScene, filePath);

			m_editorScenePath = filePath;
		}
	}
	void EditorLayer::serializeScene(Ref<Scene> scene, const std::string& filePath)
	{
		SceneSerializer sceneSerializer(scene);
		sceneSerializer.serializeToText(filePath);
	}
	bool EditorLayer::deserializeScene(Ref<Scene> dstScene, const std::string& filePath)
	{
		SceneSerializer sceneSerializer(dstScene);
		return sceneSerializer.deserializeText(filePath);
	}
	void EditorLayer::onScenePlay()
	{
		m_sceneState = SceneState::Play;

		//Create a copy of the editorScene
		m_runtimeScene = CreateRef<Scene>(*m_editorScene);

		//Cache the editor scene's selected entity before setting up a new runtime scene context for the sceneHierarchyPanel
		m_editorSceneSelectedEntityCache = m_sceneHierarchyPanel.getSelectedEntity();
		//Set the context to be the new copied/temp scene (m_runtimeScene)
		m_sceneHierarchyPanel.setSceneContext(m_runtimeScene);

		m_runtimeScene->onRuntimeStart();

	}
	void EditorLayer::onSceneStop()
	{
		m_sceneState = SceneState::Edit;

		m_runtimeScene->onRuntimeStop();

		//Set back the context to be the original scene (m_editorScene)
		m_sceneHierarchyPanel.setSceneContext(m_editorScene);
		//Set back the selected entity for the editor scene context
		m_sceneHierarchyPanel.setSelectedEntity(m_editorSceneSelectedEntityCache);

		//Delete the copied/runtimeScene (Decrementing the shared_ptr reference count results in deleting it if there is no other references)
		m_runtimeScene = nullptr;

	}
	void EditorLayer::onDuplicateEntity()
	{
		//Make sure we are in edit mode
		if (m_sceneState != SceneState::Edit)
			return;
		
		Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		//Check if there is an entity selected
		if (selectedEntity)  //NOTE: Entity has an overload for the bool operator
		{
			m_editorScene->duplicateEntity(selectedEntity);
		}
		
	}
}