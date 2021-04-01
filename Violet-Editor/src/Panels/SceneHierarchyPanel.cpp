#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include "Violet/Scene/Components.h"

namespace Violet {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		setContextScene(scene);
	}
	void SceneHierarchyPanel::setContextScene(const Ref<Scene>& scene)
	{
		m_contextScene = scene;
	}
	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//Iterate over all the entities in the scene
		m_contextScene->m_registry.each([&](entt::entity entityID)
		{
				Entity entity{ entityID, m_contextScene.get() };
				drawEntityNode(entity);
		});

		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow; //Make it expandable when clicking the arrow rather than the whole text
		flags |= (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;  //Make entity as the selected entity if it matches with m_selectEntity
		//Take the entity unique ID and serve it as an ID for the TreeNode UI component
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)(entt::entity)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectedEntity = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
}