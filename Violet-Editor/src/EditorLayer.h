#pragma once
#include <Violet.h>
#include "Panels/SceneHierarchyPanel.h"

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
		OrthographicCameraController m_cameraController;

		glm::vec3 m_objectPosition{ 0.0f, 0.0f, 0.0f };
		float m_objectRotationZ = 0.0f;
		glm::vec3 m_objectScale{ 1.0f, 1.0f, 1.0f };
		glm::vec4 m_objectColor = { 1.0f, 0.7f, 0.6f, 1.0f };

		float m_translationSpeed = 0.1f;
		float m_scaleSpeed = 0.3f;

		Ref<Texture2D> m_LetterVTexture;
		Ref<Texture2D> m_transparentTexture;
		Ref<Texture2D> m_grassTexture;
		Ref<FrameBuffer> m_frameBuffer;

		bool m_viewPortFocused = false, m_viewPortHovered = false;
		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };

		Ref<Scene> m_activeScene;

		Entity m_squareEntity;
		Entity m_cameraEntity;

		/*Panels*/
		SceneHierarchyPanel m_sceneHierarchyPanel;

	};

}