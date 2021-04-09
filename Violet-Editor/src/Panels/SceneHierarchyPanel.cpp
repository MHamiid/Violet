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
		m_selectedEntity = {};  //Reset the selected entity
		if (m_propertiesPanelContext) //If there is a properties panel attached
		{
			m_propertiesPanelContext->setEntityContext({ });  //Update the selected entity to the PropertiesPanel
		}
	}
	void SceneHierarchyPanel::setPropertiesPanelContext(PropertiesPanel* propertiesPanel)
	{
		m_propertiesPanelContext = propertiesPanel;
	}
	void SceneHierarchyPanel::onImGuiRender(bool disableInteraction)
	{
		ImGui::Begin("Scene Hierarchy");

		//Iterate over all the entities in the scene
		m_sceneContext->m_registry.each([&](entt::entity entityID)
		{
				Entity entity{ entityID, m_sceneContext.get() };
				drawEntityNode(entity, disableInteraction);
		});

		//If a blank space in the window pressed unselect the selected entity
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_selectedEntity = {};

			if (m_propertiesPanelContext) //If there is a properties panel attached
			{
				//Update the properties panel entity context
				m_propertiesPanelContext->setEntityContext({ });
			}
		}

		/*Right-Click Pop-Up Menu*/
		//If the interaction is not disabled and right-clicked in the Scene Hierarchy panel but not on an item (the entities) ====> clicking on a blank space in the panel, Note the checking sequence of the conditions (Pop-up will not render if the first condition failed) 
		if (!disableInteraction && ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity")) //If pressed
			{
				m_sceneContext->createEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}


		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity, bool disableInteraction)
	{
		const auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow; //Make it expandable when clicking the arrow rather than the whole text
		flags |= (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;  //Make entity as the selected entity if it matches with m_selectEntity
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;  //Make the whole node selectable rather than the text area of the node
		//Take the entity unique ID and serve it as an ID for the TreeNode UI component
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectedEntity = entity;

			if (m_propertiesPanelContext) //If there is a properties panel attached
			{
				//Update the properties panel entity context
				m_propertiesPanelContext->setEntityContext(entity);
			}
		}

		/*Right-Click Pop-Up Item*/
		//If the interaction is not disabled and right-clicked on the item, Note the checking sequence of the conditions (Pop-up will not render if the first condition failed) 
		if (!disableInteraction && ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity")) //If pressed
			{
				m_selectedEntity = {};
				if (m_propertiesPanelContext) //If there is a properties panel attached
				{
					m_propertiesPanelContext->setEntityContext({});  //Set an empty (non-valid) entity for the properties panel ====> stop the propeties panel from rendering the entity's components
					m_sceneContext->destroyEntity(entity);
				}
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
}