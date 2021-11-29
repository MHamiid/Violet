#include "PropertiesPanel.h"
#include <imgui.h>
#include "Violet/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>
#include <filesystem>
#include "Violet/Utils/PlatformUtils.h"
#include "Violet/ImGui/ImGuiUtils.h"

namespace Violet {

	//TODO: Note that the assets path is hardcoded, extract the assets path from the Project (Project class not created yet).
	//g_assetsPath declaration
	extern const std::filesystem::path g_assetsPath; // [HARD-CODED][TEMP][GLOBAL]

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
		if (m_entityContext) //If there is a context entity selected and it is a valid entity (@exmple: not deleted)
		{
			drawComponents(m_entityContext);
		}

		ImGui::End();
	}

	/*TODO: Move It To A UI Library*/
	static bool DrawVec3ControlUI(const std::string& label, glm::vec3& values, float valueStepSize = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f) 
	{
		/*TODO: Set ImGui::DragFloat to show the full value (no number rounding) when hovered*/
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f }); //Set spacing between the items to 0

		/*Button Size*/
		float characterHeight = (GImGui->Font->FontSize) + (GImGui->Style.FramePadding.y * 2.0f);
		ImVec2 buttonSize = { characterHeight + 3.0f, characterHeight };

		/*First Item*/
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) //If Pressed 
		{
			values.x = resetValue;
			isValueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if (ImGui::DragFloat("##X-ID", &values.x, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##X ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		/*Second Item*/
		ImGui::SameLine();
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) //If Pressed 
		{
			values.y = resetValue;
			isValueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if(ImGui::DragFloat("##Y-ID", &values.y, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##Y ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		/*Third Item*/
		ImGui::SameLine();
		/*Set Button Style Color*/
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.7f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) //If Pressed 
		{
			values.z = resetValue;
			isValueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3); //Pop the three defined style colors for the button

		ImGui::SameLine();
		if(ImGui::DragFloat("##Z-ID", &values.z, valueStepSize, 0.0f, 0.0f, "%.2f")) { isValueChanged = true; }  //## in ##Z ====> Don't show the label, %.2f ====> Show two decimal values
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1); //Reset it back to 1

		ImGui::PopID();

		return isValueChanged;
	}

	
	/*TODO: Set a better way for creating a unique ID for ImGui::TreeNodeEx rather than typeid(class).hash_code()*/
	template<typename ComponentType, typename UIFunction>
	void PropertiesPanel::drawComponent(const std::string& name, Entity entity, UIFunction UIFUNC)
	{
		if (entity.hasComponent<ComponentType>()) {
			ComponentType& component = entity.getComponent<ComponentType>();

			ImVec2 fullContentRegionAvailable = ImGui::GetContentRegionAvail();
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
				| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 3.0f, 3.0f });

			float characterHeight = (GImGui->Font->FontSize) + (GImGui->Style.FramePadding.y * 2.0f);
			ImGui::Separator();
			bool treeNodeOpened = ImGui::TreeNodeEx((void*)typeid(ComponentType).hash_code(), treeNodeFlags, name.c_str()); //Render the node

			ImGui::PopStyleVar();

			ImGui::SameLine(fullContentRegionAvailable.x - characterHeight * 0.5f);
			if (ImGui::Button("...", ImVec2{ characterHeight, characterHeight })) {
				ImGui::OpenPopup("ComponentSetting-ID");
			}

			bool setComponentToBeRemoved = false;
			if (ImGui::BeginPopup("ComponentSetting-ID")) {
				if (ImGui::MenuItem("Remove Component")) {
					setComponentToBeRemoved = true;
				}
				ImGui::EndPopup();
			}

			if (treeNodeOpened) //If opened display the component
			{
				UIFUNC(component);
				ImGui::TreePop();
			}
			if (setComponentToBeRemoved) 
			{
				entity.removeComponent<ComponentType>(); 
			}

		}

	}


	void PropertiesPanel::drawComponents(Entity entity)
	{
		/*Search for all the possible components if they exist*/
		
		/*Tag Component*/
		if (entity.hasComponent<TagComponent>()) {
			auto& tag = entity.getComponent<TagComponent>().tag;

			ImGui::InputText("##Tag-ID", &tag);
		}

		/*Add Component Button*/
		ImGui::SameLine();
		ImGui::PushItemWidth(-1); //Set the width to be the remaining available width on the line
		drawAddComponentButton();
		ImGui::PopItemWidth();

		/*Transform Component*/
		drawComponent<TransformComponent>("Transform", entity, [](TransformComponent& transformComponent)
		{
				DrawVec3ControlUI("Position", transformComponent.translation, 0.1f);
				glm::vec3 rotation = glm::degrees(transformComponent.rotation); //Convert it degrees to show in the UI
				if (DrawVec3ControlUI("Rotation", rotation, 0.2f)) //If any of the values changed
				{
					transformComponent.rotation = glm::radians(rotation);  //Get the rotation in degrees value form the UI and set it
				}
				DrawVec3ControlUI("Scale", transformComponent.scale, 0.1f, 1.0f);
		});

		/*Camera Component*/
		drawComponent<CameraComponent>("Camera", entity, [&](CameraComponent& cameraComponent)
			{
				auto& camera = cameraComponent.sceneCamera;

				bool isThisCameraPrimary = entity.getScene()->getPrimaryCameraEntity() == entity;
				
				Utils::ImGuiUtils::DrawWithHiddenStyle(isThisCameraPrimary, [&](bool itemHidden) 
				{

				if (ImGui::Checkbox("Primary", &isThisCameraPrimary))
				{
					entity.getScene()->setPrimaryCameraEntity(entity);
				}

				});
				
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
						if (showItemAsSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				/*Show The Properties For The Current Selected Projection*/

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
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

			});

		/*Sprit Renderer Component*/
		drawComponent<SpriteRendererComponent>("Sprite", entity, [&](SpriteRendererComponent& spritRendererComponent)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spritRendererComponent.color));

			/*Texture Thumbnail*/
			if (spritRendererComponent.texture) //If not nullptr
			{
				uint64_t textureID = spritRendererComponent.texture->getTextureID();  //Change uint32_t to uint64_t to match with the 64 bit void pointer ( ImTextureID = void* ) when casting
				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureID), ImVec2(60.0f, 60.0f), { 0, 1 }, { 1, 0 })) //Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default)
				{
					std::string textureFilePath = openTextureDialog();
					if (!textureFilePath.empty())
					{
						//Load the texture
						spritRendererComponent.texture = Texture2D::Create(textureFilePath);
					}
				}
			}
			else if(ImGui::Button("Texture", ImVec2(60.0f, 60.0f)))
			{
				std::string textureFilePath = openTextureDialog();
				if (!textureFilePath.empty())
				{
					//Load the texture
					spritRendererComponent.texture = Texture2D::Create(textureFilePath);
				}
			}

			/*Receive Dropped Payloads On The Previous Item (Texture ImageButton)*/
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* contentBrowserPayload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"); //ContentBrowser payload
				if (contentBrowserPayload) //If not nullptr
				{
					const char* textureRelativePath = reinterpret_cast<const char*>(contentBrowserPayload->Data);

					//Load the texture
					spritRendererComponent.texture = Texture2D::Create((g_assetsPath / textureRelativePath).string());
				
					ImGui::EndDragDropTarget();
				}
			}

			/*[TEMP]*/
			/*Clear Texture Button*/
			ImGui::SameLine();
			//Draw the button with hidden style (Not Clickable), if there is no texture being used on that entity
			Utils::ImGuiUtils::DrawWithHiddenStyle(spritRendererComponent.texture == nullptr, [&](bool itemHidden)
				{
					//Button will be drawn with the normal style (Clickable), if spritRendererComponent.texture != nullptr ===> There is a texture being used on that entity
					if (ImGui::Button("Clear", ImVec2(40.0f, 25.0f)))
					{
						spritRendererComponent.texture.reset();
					}
				});

			//Controlling the texture's size factor
			ImGui::DragFloat("Size Factor", &spritRendererComponent.textureSizeFactor, 0.1f, 0.0f, 100.0f);
		});

		/*Circle Renderer Component*/
		drawComponent<CircleRendererComponent>("Circle Renderer", entity, [&](CircleRendererComponent& circleRendererComponent)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(circleRendererComponent.color));
				ImGui::DragFloat("Thickness", &circleRendererComponent.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &circleRendererComponent.Fade, 0.00025f, 0.0f, 1.0f);
		
			});

		/*RidgidBody2D Component*/
		drawComponent<RidgidBody2DComponent>("RidgidBody2D", entity, [&](RidgidBody2DComponent& rb2dComponent)
			{
				const char* bodyTypeStrings[3] = { "Static", "Dynamic", "Kinematic" };  //NOTE: Arrange this array the same as the RidgidBody2DComponent::BodyType enum class
				const char* currentBodyTypeString = bodyTypeStrings[(int)rb2dComponent.Type];  //Get the body type

				//ComboBox
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))  //If opened
				{
					for (int i = 0; i < 3; i++) {
						bool showItemAsSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], showItemAsSelected))
						{
							rb2dComponent.Type = (RidgidBody2DComponent::BodyType)i;
						}
						if (showItemAsSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				//Control Fixed Rotation parameter
				ImGui::Checkbox("Fixed Rotation", &rb2dComponent.FixedRotation);
			});

		/*BoxCollider2D Component*/
		drawComponent<BoxCollider2DComponent>("BoxCollider2D", entity, [&](BoxCollider2DComponent& bc2dComponent)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(bc2dComponent.Offset));
				ImGui::DragFloat2("Size Factor", glm::value_ptr(bc2dComponent.SizeFactor));
				ImGui::DragFloat("Density", &bc2dComponent.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &bc2dComponent.Friction, 0.01f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);      //[From b2Fixture.h] /// The friction coefficient, usually in the range [0,1].
				ImGui::DragFloat("Restitution", &bc2dComponent.Restitution, 0.01f, 0.0f, 1.0f); 	//[From b2Fixture.h] /// The restitution (elasticity) usually in the range [0,1].
				ImGui::DragFloat("Restitution Threshold", &bc2dComponent.RestitutionThreshold, 0.01f, 0.0f);
			});


	}

	void PropertiesPanel::drawAddComponentButton()
	{
		/*Check If The Entity Has All The Possible Components*/
		bool entityHasAllComponents = m_entityContext.hasComponent<TransformComponent>() && m_entityContext.hasComponent<SpriteRendererComponent>()
			&& m_entityContext.hasComponent<CameraComponent>();
		
		Utils::ImGuiUtils::DrawWithHiddenStyle(entityHasAllComponents, [](bool itemHidden)
		{
			if (ImGui::Button("Add Component")) //If pressed open the pop-up
			{
				ImGui::OpenPopup("AddComponent-ID");
			}
		});

		if (ImGui::BeginPopup("AddComponent-ID")) //Create the pop-up
		{
			if (!m_entityContext.hasComponent<TransformComponent>() && ImGui::MenuItem("Transform"))  //Menu item will not be rendered if the entity already has the component, Note the sequence of the checking of the conditions
			{
				m_entityContext.addComponent<TransformComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (!m_entityContext.hasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite"))
			{
				m_entityContext.addComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if(!m_entityContext.hasComponent<CircleRendererComponent>() && ImGui::MenuItem("Circle Renderer"))
			{
				m_entityContext.addComponent<CircleRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (!m_entityContext.hasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
			{
				m_entityContext.addComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (!m_entityContext.hasComponent<RidgidBody2DComponent>() && ImGui::MenuItem("RidgidBody2D"))
			{
				m_entityContext.addComponent<RidgidBody2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (!m_entityContext.hasComponent<BoxCollider2DComponent>() && ImGui::MenuItem("BoxCollider2D"))
			{
				m_entityContext.addComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	std::string PropertiesPanel::openTextureDialog()
	{
		std::string textureFilePath = FileDialogs::OpenFile("Image files(*.jpg; *.png; *.bmp)\0 * .jpg; *.png; *.bmp\0All files\0*.*\0");

		return textureFilePath;
	}

}