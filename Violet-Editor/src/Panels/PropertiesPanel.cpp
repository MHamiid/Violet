#include "PropertiesPanel.h"
#include <imgui.h>
#include "Violet/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>

namespace Violet {

	PropertiesPanel::PropertiesPanel(Entity entity)
	{
		setEntityContext(entity);
	}

	void PropertiesPanel::setEntityContext(Entity entity)
	{
		m_entityContext = entity;
	}

	void PropertiesPanel::onImGuiRender()
	{
		/*Properties Panel*/
		ImGui::Begin("Properties");
		if (m_entityContext) //If there is a context entity selected
		{
			drawComponents(m_entityContext);
		}

		ImGui::End();
	}

	void PropertiesPanel::drawComponents(Entity entity)
	{
		/*Search for all the possible components if they exist*/
		
		/*Tag Component*/
		if (entity.hasComponent<TagComponent>()) {
			auto& tag = entity.getComponent<TagComponent>().tag;

			ImGui::InputText("Tag", &tag);
		}

		/*Transform Component*/
		if (entity.hasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity
				, ImGuiTreeNodeFlags_DefaultOpen, "Transform")) //If opened display the component
			{
				auto& transformComponent = entity.getComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.translation), 0.1f);
				ImGui::TreePop();
			}

		}

		/*Camera Component*/
		if (entity.hasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity
				, ImGuiTreeNodeFlags_DefaultOpen, "Camera")) //If opened display the component
			{
				auto& cameraComponent = entity.getComponent<CameraComponent>();
				auto& camera = cameraComponent.sceneCamera;

				bool isThisCameraPrimary = entity.getScene()->getPrimaryCameraEntity() == entity;
				bool checkBoxHidden = false;
				if (isThisCameraPrimary)
				{
					/*Draw With Hidden Style*/
					checkBoxHidden = true;
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				
				if (ImGui::Checkbox("Primary", &isThisCameraPrimary))
				{
					entity.getScene()->setPrimaryCameraEntity(CreateRef<Entity>(entity));
				}

				if (checkBoxHidden)
				{
					/*Pop Drawing With Hidden Style*/
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				const char* projectionTypeStrings[2] = { "Perspective", "Orthographic" };  //NOTE: Arrange this array the same as the SceneCamera::ProjectionType enum class
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];  //Get the current sceneCamera projection

				//ComboBox
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))  //If opened
				{
					for (int i = 0; i < 2; i++) {
						bool showItemAsSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], showItemAsSelected))
						{
							/* Set sceneCamera new projection.
							 * NOTE: setProjectionType function ignores setting the projection
							 * and the calculation of the new projection matrix if the new projection is same as the old projection.
							 */
							camera.setProjectionType((SceneCamera::ProjectionType)i);
						}
						if(showItemAsSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				/*Show The Properties For The Current Selected Projection*/

				if(camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVertivalFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
					if (ImGui::DragFloat("VFOV", &perspectiveVertivalFOV, 0.05f)) //If Dragfloat changed the value 
					{
						camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVertivalFOV));
					}

					float perspectiveNear = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear, 0.1f)) //If Dragfloat changed the value 
					{
						camera.setPerspectiveNearClip(perspectiveNear);
					}

					float perspectiveFar = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar, 0.1f)) //If Dragfloat changed the value 
					{
						camera.setPerspectiveFarClip(perspectiveFar);
					}
				}
				else if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic) 
				{
					float orthoSize = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.05f)) //If Dragfloat changed the value 
					{
						camera.setOrthographicSize(orthoSize);
					}
					
					float orthoNear = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear, 0.01f)) //If Dragfloat changed the value 
					{
						camera.setOrthographicNearClip(orthoNear);
					}

					float orthoFar = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar, 0.01f)) //If Dragfloat changed the value 
					{
						camera.setOrthographicFarClip(orthoFar);
					}
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}


				ImGui::TreePop();
			}

		}

		/*Sprit Renderer Component*/
		if (entity.hasComponent<SpriteRendererComponent>()) {
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entt::entity)entity
				, ImGuiTreeNodeFlags_DefaultOpen, "Sprite")) //If opened display the component
			{
				auto& spritRendererComponent = entity.getComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spritRendererComponent.color));
				ImGui::TreePop();
			}

		}

	}

}