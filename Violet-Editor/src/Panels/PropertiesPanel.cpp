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

	/*TODO: Move It To A UI Library*/
	static bool drawVec3ControlUI(const std::string& label, glm::vec3& values, float valueStepSize = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f) 
	{
		bool isValueChanged = false;

		//Set an ID for this Whole Component UI, So that this function can be used more than once without ImGui::DragFloat being related to each other, DragFloat ID is repeated (##X, ##Y, ##Z)
		ImGui::PushID(label.c_str());
									
		ImGui::Columns(2);
		
		/*First Column (The Label)*/
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());  
		
		ImGui::NextColumn();

		/*Second Column*/

		/*Set Second Column Style*/
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth()); //Set to have 3 items for the width of the second column
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); //Set spacing between the items to 0


		/*First Item*/
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
		if (ImGui::Button("X")) //If Pressed 
		{
			values.x = resetValue;
			isValueChanged = true;
		}
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##X ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		/*Second Item*/
		ImGui::SameLine();
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
		if (ImGui::Button("Y")) //If Pressed 
		{
			values.y = resetValue;
			isValueChanged = true;
		}
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if(ImGui::DragFloat("##Y", &values.y, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##Y ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		/*Third Item*/
		ImGui::SameLine();
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.7f, 1.0f));
		if (ImGui::Button("Z")) //If Pressed 
		{
			values.z = resetValue;
			isValueChanged = true;
		}
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if(ImGui::DragFloat("##Z", &values.z, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##Z ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1); //Reset it back to 1

		ImGui::PopID();

		return isValueChanged;
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
				drawVec3ControlUI("Position", transformComponent.translation, 0.1f);
				glm::vec3 rotation = glm::degrees(transformComponent.rotation); //Convert it degrees to show in the UI
				if (drawVec3ControlUI("Rotation", rotation, 0.2f)) //If any of the values changed
				{
					transformComponent.rotation = glm::radians(rotation);  //Get the rotation in degrees value form the UI and set it
				}
				drawVec3ControlUI("Scale", transformComponent.scale, 0.1f, 1.0f);
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