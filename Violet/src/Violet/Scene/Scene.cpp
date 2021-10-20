#include "VIOPCH.h"
#include "Scene.h"
#include "Violet/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"
#include "Script.h"
/*BOX2D*/
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Violet {
	/*Helper Functions*/
	//Conversion function between Box2D body type and our RidgidBody2D type
	static b2BodyType RidgidBody2DTypeToBox2DType(RidgidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case RidgidBody2DComponent::BodyType::Static:	 return b2_staticBody;
		case RidgidBody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
		case RidgidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		VIO_CORE_ASSERT(false, "[Scene] Unknown Body Type");
		return b2_staticBody;
	}

}
namespace Violet {
	Scene::Scene(const std::string& sceneName) : m_sceneName(sceneName)
	{
		//Allocate space to copy the entity to when setPrimaryCameraEntity
		m_primaryCameraEntity = CreateScope<Entity>();
	}

	Scene::Scene(const Scene& other)
	{
		m_sceneName = other.m_sceneName;
		m_viewPortWidth = other.m_viewPortWidth;
		m_viewPortHeight = other.m_viewPortHeight;

		std::unordered_map<UUID, entt::entity> dstEnttMap;  //A map that links the new copied/created entities' entt:entity (enttID) with UUID

		/*Copy Entities*/

		const entt::registry& srcRegistryCONST = other.m_registry;  //NOTE: Scene& other is const
		//Remove the constness from the registry so we can use a view into the registry, otherwise it would result in a compile error when trying to use a view into the registry
		entt::registry& srcRegistry = const_cast<entt::registry&>(srcRegistryCONST);

		//Get all the entities in the registry (getting a view for IDComponents to get access to all the entities as that any created entity must have IDComponent by default)
		auto idView = srcRegistry.view<IDComponent>();
		for (auto srcEnttID : idView)
		{
			UUID uuid = other.m_registry.get<IDComponent>(srcEnttID).ID;
			const std::string& tagName = other.m_registry.get<TagComponent>(srcEnttID).tag;

			//Create the entity
			Entity dstEntity = createEntityWithUUID(uuid, tagName);
			//Register the new create entity's entt::entity (enttID) into the map with it's corresponding UUID
			dstEnttMap[uuid] = static_cast<entt::entity>(dstEntity);

			//Finding the primaryCameraEntity
			if (srcEnttID == *other.m_primaryCameraEntity)
			{
				m_primaryCameraEntity = CreateScope<Entity>(dstEntity);
			}
		}

		/*Copy Components (except IDComponent and TagComponent [they are already copied when the entity is created using createEntityWithUUID()])
		* NOTE: createEntityWithUUID() also adds TransfromComponent, however we still want to copy the values/data in the TransfromComponent
		*/
		copyComponent<TransformComponent>(srcRegistry, dstEnttMap);
		copyComponent<SpriteRendererComponent>(srcRegistry, dstEnttMap);
		copyComponent<CameraComponent>(srcRegistry, dstEnttMap);
		copyComponent<NativeScriptComponent>(srcRegistry, dstEnttMap);
		copyComponent<RidgidBody2DComponent>(srcRegistry, dstEnttMap);
		copyComponent<BoxCollider2DComponent>(srcRegistry, dstEnttMap);

	}

	Scene::~Scene()
	{
	}

	void Scene::onRuntimeStart()
	{
		//Start Box2D world
		m_b2PhysicsWorld = new b2World({0.0f, -9.8f});  //Set the gravity to real world gravity -9.8 m/s

		auto view = m_registry.view<RidgidBody2DComponent>();
		//Iterate over all the entities that have RidgitBody2DComponent
		for (auto enttEntity : view)
		{
			Entity entity(enttEntity, this); //Create an object of our own Entity from enttEntity to use our Entity API features and functions
			
			/*Pull Up Data From The Entity*/
			TransformComponent& transformComponent = entity.getComponent<TransformComponent>();
			RidgidBody2DComponent& rb2dComponent = entity.getComponent<RidgidBody2DComponent>();

			/*Create Bodies Into The Physics World*/
			b2BodyDef bodyDef;
			bodyDef.type = RidgidBody2DTypeToBox2DType(rb2dComponent.Type);
			bodyDef.position.Set(transformComponent.translation.x, transformComponent.translation.y);
			bodyDef.angle = transformComponent.rotation.z;

			b2Body* body = m_b2PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2dComponent.FixedRotation);

			rb2dComponent.RuntimeBody = body;   //Store a pointer to the created b2body in the RidgidBody2DComponent
			
			if (entity.hasComponent<BoxCollider2DComponent>())
			{
				BoxCollider2DComponent& bc2dComponent = entity.getComponent<BoxCollider2DComponent>();

				//Create a shape for the collider
				b2PolygonShape colliderShape;
				colliderShape.SetAsBox(bc2dComponent.SizeFactor.x * transformComponent.scale.x, bc2dComponent.SizeFactor.y * transformComponent.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &colliderShape;
				fixtureDef.density = bc2dComponent.Density;
				fixtureDef.friction = bc2dComponent.Friction;
				fixtureDef.restitution = bc2dComponent.Restitution;
				fixtureDef.restitutionThreshold = bc2dComponent.RestitutionThreshold;
				
				body->CreateFixture(&fixtureDef);  //Add the fixture to the body
			}
		}

	}

	void Scene::onRuntimeStop()
	{
		delete m_b2PhysicsWorld;
		m_b2PhysicsWorld = nullptr;
	}

	void Scene::onUpdateRuntime(DeltaTime deltaTime)
	{
		/*NOTE: The order of updating the runtime is: Scripts ===> Physics ===> Render
		, as each step can affect the outcome of the next step and it won't be picked up until next frame if it was not for that order*/

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

		/*Update Physics*/
		//TODO: Expose these variables to the editor as physics settings
		//These variables control the trade of between performance and precision (ex: defines how often calculations are done)
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;

		m_b2PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);  //Simulate the physics in the world

		/*Retrieve the transfromation from Box2D after the physics simulation is done*/

		auto view = m_registry.view<RidgidBody2DComponent>();
		//Iterate over the entities that the physics simulation has the ability to affect them
		for (auto enttEntity : view)
		{
			Entity entity(enttEntity, this); //Create an object of our own Entity from enttEntity to use our Entity API features and functions

			RidgidBody2DComponent& rb2dComponent = entity.getComponent<RidgidBody2DComponent>();
			TransformComponent& transfromComponent = entity.getComponent<TransformComponent>();

			/*Pull up the transfromation data from Box2D and update the entity's transformation*/
			b2Body* body = reinterpret_cast<b2Body*>(rb2dComponent.RuntimeBody);
			const b2Vec2& position = body->GetPosition();
			transfromComponent.translation.x = position.x;
			transfromComponent.translation.y = position.y;
			transfromComponent.rotation.z = body->GetAngle();
		}


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
		return createEntityWithUUID(UUID(), tagName); //Generate a new UUID
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& tagName)
	{
		/*All entities that are created come with IDComponent, TransformComponent and TagComponent*/

		Entity entity = { m_registry.create(), this };

		entity.addComponent<IDComponent>(uuid);
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

	/*
	* Copies a component from another registry that has the same entities (same UUID)
	* std::unordered_map<UUID, entt::entity>& dstEnttMap ===> maps the UUID to the destination entt::entity (enttID), UUID is the only link between the src registry and dst registry (this->m_registry)
	*/
	template<typename Component>
	void Scene::copyComponent(entt::registry& src, const std::unordered_map<UUID, entt::entity>& dstEnttMap)
	{
		//NOTE: Using UUID to match the entities between dst and src (dst registry == this->m_registry)

		auto view = src.view<Component>();
		for (auto srcEnttID : view)
		{
			UUID srcUUID = src.get<IDComponent>(srcEnttID).ID;
			VIO_CORE_ASSERT(dstEnttMap.find(srcUUID) != dstEnttMap.end(), "[Scene] Source Registry Entity UUID Not Found In The Destination Map!");
			entt::entity dstEnttID = dstEnttMap.at(srcUUID);

			auto& component = src.get<Component>(srcEnttID);

			/*Copy the component*/
			//NOTE: Using emplace_or_replace<>() instead of emplace<>(), incase the component already exists for that entity (as for the TransformComponent, which already exists)
			m_registry.emplace_or_replace<Component>(dstEnttID, component);   //Forwards the component to the copy constructor to copy it
		}
	}

	/*Specialized Templates*/
	/*Must Add A Definition For Every Possible Component*/
	template<>
	void Scene::onComponentAdded<IDComponent>(Entity entity, IDComponent& IDComponent)
	{
	}

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

	template<>
	void Scene::onComponentAdded<RidgidBody2DComponent>(Entity entity, RidgidBody2DComponent& rb2dComponent)
	{
	}

	template<>
	void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& bc2dComponent)
	{
	}
}