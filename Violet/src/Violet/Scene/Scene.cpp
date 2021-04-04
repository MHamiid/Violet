#include "VIOPCH.h"
#include "Scene.h"
#include "Violet/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

namespace Violet {
	/*TODO: Set the SceneCameras viewport on creation*/
	Scene::Scene()
	{
		//Allocate space to copy the entity to when setPrimaryCameraEntity
		m_primaryCameraEntity = new Entity();
	}

	Scene::~Scene()
	{
		delete m_primaryCameraEntity;
	}

	void Scene::onUpdate(DeltaTime deltaTime)
	{
		/*Update Scripts*/
		//Get all script components
		m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {
			//TODO: Move to be called when Game runtime starts (Scene::onScenePlay)
			if (!nativeScriptComponent.script) //If first time calling the script 
			{
				nativeScriptComponent.script = nativeScriptComponent.instantiateScriptFUNC(); //Create a script instance
				nativeScriptComponent.script->m_entity = Entity{ entity, this }; //this refers to this scene
				nativeScriptComponent.script->onCreate();  //TODO: Remove to run on game runtime startup
			}

			nativeScriptComponent.script->onUpdate(deltaTime);

			}
		
		); 

		/*Render 2D*/
		//If entity has been assigned to a scene and entity has a CameraComponent attached, Note the sequence of the checking of the conditions
		if (m_primaryCameraEntity->isValidEntity() && m_primaryCameraEntity->hasComponent<CameraComponent>())
		{
			CameraComponent& primaryCameraComponent = m_primaryCameraEntity->getComponent<CameraComponent>();
			//Render only if there is a valid camera in the primaryCameraEntity
			if (primaryCameraComponent.sceneCamera.getProjectionMatrix() != glm::mat4(1.0f)) //ToBeThoughtOf //If the projection matrix has been set and not equal to the default identity matrix glm::mat4(1.0f)
			{
				/*Render 2D*/
				Renderer2D::BeginScene(primaryCameraComponent.sceneCamera, m_primaryCameraEntity->getComponent<TransformComponent>().getTransform());
				entt::basic_group spriteGroup = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

				for (auto entity : spriteGroup) {
					auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(transform.getTransform(), sprite.color);
				}
				Renderer2D::EndScene();
			}
		}

	}

	void Scene::onViewPortResize(uint32_t width, uint32_t height)
	{
		m_viewPortWidth = width;
		m_viewPortHeight = height;

		/*Resize Non-FixedAspectRatio Cameras*/
		auto cameraComponentsView = m_registry.view<CameraComponent>(); //Get all the camera components
		for (auto entity : cameraComponentsView) {
			CameraComponent& cameraComponent = cameraComponentsView.get<CameraComponent>(entity);
			
			if (!cameraComponent.fixedAspectRatio) {
				cameraComponent.sceneCamera.setViewPortSize(width, height);
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

	void Scene::setPrimaryCameraEntity(Entity cameraEntity)
	{
		*m_primaryCameraEntity = cameraEntity;
	}

	Entity Scene::getPrimaryCameraEntity()
	{
		return *m_primaryCameraEntity;
	}

}