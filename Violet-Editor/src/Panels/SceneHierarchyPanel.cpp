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

			//Update the properties panel entity context
			m_propertiesPanelContext->setEntityContext({ });
		}

		/*Right-Click Pop-Up Menu*/
		if (ImGui::BeginPopupContextWindow(0, 1, false)) //If right-clicked in the Scene Hierarchy panel but not on an item (the entities) ====> clicking on a blank space in the panel
		{
			if (ImGui::MenuItem("Create Empty Entity")) //If pressed
			{
				m_sceneContext->createEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}


		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow; //Make it expandable when clicking the arrow rather than the whole text
		flags |= (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;  //Make entity as the selected entity if it matches with m_selectEntity
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;  //Make the whole node selectable rather than the text area of the node
		//Take the entity unique ID and serve it as an ID for the TreeNode UI component
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectedEntity = entity;

			//Update the properties panel entity context
			m_propertiesPanelContext->setEntityContext(entity);
		}

		/*Right-Click Pop-Up Item*/
		if (ImGui::BeginPopupContextItem()) //If right-clicked on the item
		{
			if (ImGui::MenuItem("Delete Entity")) //If pressed
			{
				m_propertiesPanelContext->setEntityContext({});  //Set an empty (non-valid) entity for the properties panel ====> stop the propeties panel from rendering the entity's components
				m_sceneContext->destroyEntity(entity);
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
}