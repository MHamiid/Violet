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
	private:
		void newScene();
		void openScene();
		void saveSceneAs();
	private:
		OrthographicCameraController m_cameraController;

		Ref<Texture2D> m_LetterVTexture;
		Ref<Texture2D> m_transparentTexture;
		Ref<Texture2D> m_grassTexture;
		Ref<FrameBuffer> m_frameBuffer;

		bool m_viewPortFocused = false, m_viewPortHovered = false;
		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };

		Ref<Scene> m_activeScene;

		/*Panels*/
		SceneHierarchyPanel m_sceneHierarchyPanel;
		PropertiesPanel m_propertiesPanel;

	};

}