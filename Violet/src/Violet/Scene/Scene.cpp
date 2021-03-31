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
		entt::basic_group group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform.getTransform(), sprite.color);
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