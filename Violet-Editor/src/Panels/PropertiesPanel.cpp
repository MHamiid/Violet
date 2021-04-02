#include "PropertiesPanel.h"
#include <imgui.h>
#include "Violet/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include <misc/cpp/imgui_stdlib.h>

namespace Violet {

	PropertiesPanel::PropertiesPanel(Entity entity)
	{
		setContextEntity(entity);
	}

	void PropertiesPanel::setContextEntity(Entity entity)
	{
		m_contextEntity = entity;
	}

	void PropertiesPanel::onImGuiRender()
	{
		/*Properties Panel*/
		ImGui::Begin("Properties");
		if (m_contextEntity) //If there is a context entity selected
		{
			drawComponents(m_contextEntity);
		}

		ImGui::End();
	}

	void PropertiesPanel::drawComponents(Entity entity)
	{
		/*Search for all the possible components if they exist*/
		if (entity.hasComponent<TagComponent>()) {
			auto& tag = entity.getComponent<TagComponent>().tag;

			ImGui::InputText("Tag", &tag);
		}

		if (entity.hasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity
				, ImGuiTreeNodeFlags_DefaultOpen, "Transform")) //If opened display the component
			{
				auto& transform = entity.getComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform.translation), 0.1f);
				ImGui::TreePop();
			}

		}
	}

}