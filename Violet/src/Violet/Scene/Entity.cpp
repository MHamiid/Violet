#include "VIOPCH.h"
#include "Entity.h"

namespace Violet {
	
	Entity::Entity(entt::entity enttID, Scene* scene) : m_enttID(enttID), m_scene(scene)
	{
	}

}