#include "VIOPCH.h"
#include "Scene.h"
#include "Violet/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Violet {

	Scene::Scene()
	{
		//NOTE: ent::entity is a uint32_t ID
		entt::entity entity = m_registry.create();
	}

	Scene::~Scene()
	{
	}

	void Scene::onUpdate(DeltaTime deltaTime)
	{
		if (m_primaryCameraEntity) {
			CameraComponent& primaryCameraComp = m_primaryCameraEntity->getComponent<CameraComponent>();
			//Render only if there is a valid camera in the primaryCameraEntity
			if (primaryCameraComp.camera.getProjectionMatrix() != glm::mat4(1.0f)) //If the projection matrix has been set and not equal to the default identity matrix glm::mat4(1.0f)
			{
				/*Render 2D*/
				Renderer2D::BeginScene(primaryCameraComp.camera, m_primaryCameraEntity->getComponent<TransformComponent>().getTransform());
				entt::basic_group spriteGroup = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

				for (auto entity : spriteGroup) {
					auto& [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(transform.getTransform(), sprite.color);
				}
				Renderer2D::EndScene();
			}
		}

	}

	Entity Scene::createEntity(const std::string& tagName)
	{
		/*All entities the are created come with a TransformComponent and a TagComponent*/

		Entity entity = { m_registry.create(), this };

		entity.addComponent<TransformComponent>();
		entity.addComponent<TagComponent>(tagName);

		return entity;
	}

}