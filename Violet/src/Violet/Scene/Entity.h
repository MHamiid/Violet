#pragma once
#include "Scene.h"
#include "Violet/Core/UUID.h"
#include "Components.h"

namespace Violet {

	class VIOLET_API Entity {
	public:
		Entity() = default;
		Entity(entt::entity enttID, Scene* scene);

		Entity(const Entity& other) = default;

		/*For Casting*/
		operator bool() const { return(m_enttID != entt::null) && (m_scene != nullptr); }  //Check if the entity has been set and assigned to a scene
		operator entt::entity() const { return m_enttID; }  //To use Entity as entt::entity

		/*Comparison*/
		bool operator==(const Entity& other) {
			return (m_enttID == other.m_enttID) && (m_scene == other.m_scene);
		}
		bool operator!=(const Entity& other) {
			return !(*this == other); //Make use of the overloaded operator== (previous function)
		}

		//Takes the arguments and forward them to entt's emplace directly rather than constructing them and then call entt to emplace
		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			/* 
			* Extra debug assertion for safety and easier error debugging to make sure the component was not already created
			* NOTE: entt already makes it's own assertion
			*/
			VIO_CORE_ASSERT(!hasComponent<T>(), "[Entity] Component Already Exists!");

			T& component = m_scene->m_registry.emplace<T>(m_enttID, std::forward<Args>(args)...);
			m_scene->onComponentAdded<T>(*this, component);

			return component;
		}

		//Takes the arguments and forward them to entt's emplace_or_replace directly rather than constructing them and then call entt to emplace_or_replace
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args) {
			
			T& component = m_scene->m_registry.emplace_or_replace<T>(m_enttID, std::forward<Args>(args)...);
			m_scene->onComponentAdded<T>(*this, component);

			return component;
		}

		template<typename T>
		T& getComponent() {
			/*
			* Extra debug assertion for safety and easier error debugging to make sure that the component exists
			* NOTE: entt already makes it's own assertion
			*/
			VIO_CORE_ASSERT(hasComponent<T>(), "[Entity] Component Does Not Exist!");

			return m_scene->m_registry.get<T>(m_enttID);
		}

		template<typename T>
		bool hasComponent() {
			return m_scene->m_registry.has<T>(m_enttID);
		}

		template<typename T>
		void removeComponent() {
			/*
			* Extra debug assertion for safety and easier error debugging to make sure that the component exists
			* NOTE: entt already makes it's own assertion
			*/
			VIO_CORE_ASSERT(hasComponent<T>(), "[Entity] Component Does Not Exist!");

			m_scene->m_registry.remove<T>(m_enttID);
		}

		Scene* getScene() { return m_scene; }  //TODO: Not final, think of a better approach
		UUID getUUID() { return getComponent<IDComponent>().ID; }
		const std::string& getName() { return getComponent<TagComponent>().tag; }

	private:
		entt::entity m_enttID{ entt::null }; //NOTE: ent::entity is a uint32_t ID
		Scene* m_scene = nullptr;  //The scene which the entity exists in

	};


}