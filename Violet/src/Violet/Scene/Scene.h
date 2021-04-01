#pragma once
#include <entt.hpp>
#include "Violet/Core/DeltaTime.h"

namespace Violet {
	class Entity;  //NOTE: Can't include Entity header, cause the entity includes Scene header, which result is infinite recursive include loop

	class VIOLET_API Scene {
	public:
		Scene();
		~Scene();
		void onUpdate(DeltaTime deltaTime);
		void onViewPortResize(uint32_t width, uint32_t height);
		Entity createEntity(const std::string& tagName = std::string("Empty Tag"));
		void setPrimaryCamera(Entity& cameraEntity) { m_primaryCameraEntity = &cameraEntity; }
	private:
		entt::registry m_registry; //A container for all the components and entities
		Entity* m_primaryCameraEntity = nullptr;
		uint32_t m_viewPortWidth = 0, m_viewPortHeight = 0;
	private:
		friend class Entity;
	};
}