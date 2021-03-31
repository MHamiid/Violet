#pragma once
#include "entt.hpp"
#include "Violet/Core/DeltaTime.h"
#include "Components.h"
namespace Violet {

	class VIOLET_API Scene {
	public:
		Scene();
		~Scene();
		void onUpdate(DeltaTime deltaTime);
		entt::entity createEntity() { return m_registry.create(); } //temp
		entt::registry& getRegistry() { return m_registry; }  //temp
	private:
		entt::registry m_registry; //A container for all the components and entities

	};
}