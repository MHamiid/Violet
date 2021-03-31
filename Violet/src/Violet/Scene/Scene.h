#pragma once
#include <entt.hpp>
#include "Violet/Core/DeltaTime.h"
#include "Components.h"
namespace Violet {
	class Entity;  //NOTE: Can't include Entity header, cause the entity includes Scene header, which result is infinite recursive include loop

	class VIOLET_API Scene {
	public:
		Scene();
		~Scene();
		void onUpdate(DeltaTime deltaTime);
		Entity createEntity(const std::string& tagName = std::string("Empty Tag"));
	private:
		entt::registry m_registry; //A container for all the components and entities
	private:
		friend class Entity;
	};
}