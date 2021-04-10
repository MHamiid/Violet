#include "VIOPCH.h"
#include "Scene.h"
#include "Violet/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

namespace Violet {
	Scene::Scene(const std::string& sceneName) : m_sceneName(sceneName)
	{
		//Allocate space to copy the entity to when setPrimaryCameraEntity
		m_primaryCameraEntity = CreateScope<Entity>();
	}

	Scene::~Scene()
	{
	}

	void Scene::onUpdateRuntime(DeltaTime deltaTime)
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
		//Check if the entity is valid and has been assigned to a scene and the entity has a CameraComponent attached, Note the sequence of the checking of the conditions
		if ((*m_primaryCameraEntity) && m_primaryCameraEntity->hasComponent<TransformComponent>() && m_primaryCameraEntity->hasComponent<CameraComponent>())
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

					Renderer2D::DrawSprit(transform.getTransform(), sprite, (int)entity);
				}
				Renderer2D::EndScene();
			}
		}
		else 
		{
			//Reset the statistics from the last scene when there is no camera rendering the scene
			Renderer2D::ResetSceneStatistics();
		}

	}

	void Scene::onUpdateEditor(DeltaTime deltaTime, EditorCamera& editorCamera)
	{
		/*Render 2D*/
		Renderer2D::BeginScene(editorCamera);
		entt::basic_group spriteGroup = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : spriteGroup) {
			auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprit(transform.getTransform(), sprite, (int)entity);
		}
		Renderer2D::EndScene();
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

	void Scene::destroyEntity(Entity entity)
	{
		//Check if the entity is the primary camera entity
		if (*m_primaryCameraEntity == entity) 
		{
			*m_primaryCameraEntity = Entity(); //Reset the primary camera to a non-valid entity (default entity: m_enttID = null and m_scene = nullptr) ====> stop scene rendering with the deleted entity
		}
		m_registry.destroy(entity);
	}

	void Scene::setPrimaryCameraEntity(Entity cameraEntity)
	{
		*m_primaryCameraEntity = cameraEntity;
	}

	Entity Scene::getPrimaryCameraEntity()
	{
		return *m_primaryCameraEntity;
	}

	/*Specialized Templates*/
	/*Must Add A Definition For Every Possible Component*/
	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& tagComponent)
	{
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& transformComponent)
	{
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& spriteRendererComponent)
	{
	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& cameraComponent)
	{
		//Sets the correct projection matrix for the new camera rather than creating a projection matrix with SceneCamera default member values
		if (m_viewPortWidth != 0 && m_viewPortHeight != 0)
		{
			cameraComponent.sceneCamera.setViewPortSize(m_viewPortWidth, m_viewPortHeight);
		}
		/*Set The First Entity In Our Scene With A Camera Component Attached To It As The PrimaryCameraEntity*/
		if (!(*m_primaryCameraEntity)) //If it is null 
		{
			*m_primaryCameraEntity = entity;
		}
	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& nativeScriptComponent)
	{
	}
}