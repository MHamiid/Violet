#pragma once
#include <Violet.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"

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
		void newScene(const std::string& sceneName);
		void openSceneDialog();
		void openScene(const std::string& filePath);
		void saveScene();
		void saveSceneAsDialog();
	private:

		Ref<Texture2D> m_LetterVTexture;
		Ref<Texture2D> m_transparentTexture;
		Ref<Texture2D> m_grassTexture;
		Ref<FrameBuffer> m_frameBuffer;

		bool m_viewPortFocused = false, m_viewPortHovered = false;
		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };
		glm::vec2 m_viewPortBounds[2];  //Min and max bounds, In screen space

		Ref<Scene> m_activeScene;
		EditorCamera m_editorCamera;

		/*Panels*/
		SceneHierarchyPanel m_sceneHierarchyPanel;
		PropertiesPanel m_propertiesPanel;

		int m_gizmoType = -1;  // -1 ====> No gizmo selected
		float m_snapValues[3] = { 0.5f , 45.0f, 0.5f}; //Translation, Rotation, Scale  //Snap to 0.5m for translation and scale, snap to 45 degrees for rotation

		/*New Scene Dialogue Popup*/
		bool m_showNewScenePopupModal = false;
		std::string m_newSceneNameBuffer;
		//Caching the current active scene path
		std::string m_activeScenePath;

		bool m_updateMouseSelectedEntityID = false;
	};

}