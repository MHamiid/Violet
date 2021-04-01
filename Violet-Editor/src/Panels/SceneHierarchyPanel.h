#pragma once
#include "Violet/Core/Core.h"
#include "Violet/Core/Log.h"
#include "Violet/Scene/Scene.h"
#include "Violet/Scene/Entity.h"

namespace Violet {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void setContextScene(const Ref<Scene>& scene);

		void onImGuiRender();
		
	private:
		void drawEntityNode(Entity entity);

	private:
		Ref<Scene> m_contextScene;
		Entity m_selectedEntity;
	};
}