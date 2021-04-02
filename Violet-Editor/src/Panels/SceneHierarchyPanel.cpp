#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include "Violet/Scene/Components.h"

namespace Violet {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		setSceneContext(scene);
	}
	void SceneHierarchyPanel::setSceneContext(const Ref<Scene>& scene)
	{
		m_sceneContext = scene;
	}
	void SceneHierarchyPanel::setPropertiesPanelContext(PropertiesPanel* propertiesPanel)
	{
		m_propertiesPanelContext = propertiesPanel;
	}
	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//Iterate over all the entities in the scene
		m_sceneContext->m_registry.each([&](entt::entity entityID)
		{
				Entity entity{ entityID, m_sceneContext.get() };
				drawEntityNode(entity);
		});

		//If a blank space in the window pressed unselect the selected entity
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_selectedEntity = {};

			//Update the properties panel context entity
			m_propertiesPanelContext->m_contextEntity = {};
		}

		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow; //Make it expandable when clicking the arrow rather than the whole text
		flags |= (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;  //Make entity as the selected entity if it matches with m_selectEntity
		//Take the entity unique ID and serve it as an ID for the TreeNode UI component
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectedEntity = entity;

			//Update the properties panel context entity
			m_propertiesPanelContext->m_contextEntity = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
}