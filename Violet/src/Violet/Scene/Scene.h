#pragma once
#include <entt.hpp>
#include "Violet/Core/DeltaTime.h"
#include "Violet/Renderer/EditorCamera.h"

namespace Violet {
	class Entity;  //NOTE: Can't include Entity header, cause the entity includes Scene header, which result is infinite recursive include loop

	class VIOLET_API Scene {
	public:
		Scene(const std::string& sceneName = "Untitled");
		~Scene();
		void onUpdateRuntime(DeltaTime deltaTime);
		void onUpdateEditor(DeltaTime deltaTime, EditorCamera& editorCamera);
		void onViewPortResize(uint32_t width, uint32_t height);

		Entity createEntity(const std::string& tagName = std::string("Unnamed Entity"));
		void destroyEntity(Entity entity);
	
		void setPrimaryCameraEntity(Entity cameraEntity);
		Entity getPrimaryCameraEntity();

		const std::string& getSceneName() const { return m_sceneName; }
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_registry; //A container for all the components and entities
		Scoped<Entity> m_primaryCameraEntity; //Using a pointer cause of forward declaration (Can't include Entity header here)
		uint32_t m_viewPortWidth = 0, m_viewPortHeight = 0;
		std::string m_sceneName;
	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;  //Violet-Editor
	};
}