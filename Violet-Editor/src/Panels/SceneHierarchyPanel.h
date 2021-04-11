#pragma once
#include "Violet/Core/Core.h"
#include "Violet/Core/Log.h"
#include "Violet/Scene/Scene.h"
#include "Violet/Scene/Entity.h"
#include "PropertiesPanel.h"

namespace Violet {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void setSceneContext(const Ref<Scene>& scene);
		void setPropertiesPanelContext(PropertiesPanel* propertiesPanel);

		void onImGuiRender(bool disableInteraction);
		
		void setSelectedEntity(Entity entity);
		Entity getSelectedEntity() const { return m_selectedEntity; }

	private:
		void drawEntityNode(Entity entity, bool disableInteraction);

	private:
		Ref<Scene> m_sceneContext;
		Entity m_selectedEntity;
		PropertiesPanel* m_propertiesPanelContext = nullptr;
	};
}