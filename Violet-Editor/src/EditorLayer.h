#pragma once
#include <Violet.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Violet {

	class EditorLayer : public Layer {

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		void onAttach() override;
		void onDetach() override;
		void onUpdate(DeltaTime& deltaTime) override;
		void onImGuiRender() override;
		void onEvent(Event& e) override;
	private:
		bool onKeyPressed(KeyPressedEvent& event);
		bool onMouseButtonPressed(MouseButtonPressedEvent& event);
		bool onItemsDropped(ItemsDroppedEvent& event);
	private:
		void onDuplicateEntity();
		void onOverlayRender();
		void onImGuizmoRender(Entity selectedEntity);
	private:
		void newScene(const std::string& sceneName);
		void openSceneDialog();
		void openScene(const std::string& filePath);
		void saveScene();
		void saveSceneAsDialog();

		void serializeScene(Ref<Scene> scene, const std::string& filePath);
		bool deserializeScene(Ref<Scene> dstScene, const std::string& filePath);

		void onScenePlay();
		void onSceneStop();
	private:
		/*UI & Panels*/
		void UIMenuBars();
		void UINewScenePopupModal();
		void UIToolbar();
		void UIViewport();
	private:
		/*Editor Resources*/
		/*Toolbar Textures*/
		Ref<Texture2D> m_iconPlay, m_iconStop;

		/*Test Textures*/
		Ref<Texture2D> m_LetterVTexture;
		Ref<Texture2D> m_transparentTexture;
		Ref<Texture2D> m_grassTexture;

		Ref<FrameBuffer> m_frameBuffer;

		bool m_viewPortFocused = false, m_viewPortHovered = false;
		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };
		glm::vec2 m_viewPortBounds[2] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };  //Min and max bounds without the tab bar, In screen space

		Ref<Scene> m_editorScene;  //The original scene
		Ref<Scene> m_runtimeScene; //A copy from m_editorScene when the play button is pressed (SceneState == SceneState::Play)
		EditorCamera m_editorCamera;

		/*Panels*/
		SceneHierarchyPanel m_sceneHierarchyPanel;
		PropertiesPanel m_propertiesPanel;
		ContentBrowserPanel m_contentBrowserPanel;

		/*Gizmos*/
		int m_gizmoType = -1;  // -1 ====> No gizmo selected
		float m_snapValues[3] = { 0.5f , 45.0f, 0.5f}; //Translation, Rotation, Scale  //Snap to 0.5m for translation and scale, snap to 45 degrees for rotation

		/*New Scene Dialogue Popup*/
		bool m_showNewScenePopupModal = false;
		std::string m_newSceneNameBuffer;
		//Caching the current editor scene path
		std::string m_editorScenePath;

		bool m_updateMouseSelectedEntityID = false;
		Entity m_editorSceneSelectedEntityCache;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		bool m_enableEditorScenePhysicsVisualization = true;
		bool m_enableRuntimeScenePhysicsVisualization = true;;
		SceneState m_sceneState = SceneState::Edit;
	};

}