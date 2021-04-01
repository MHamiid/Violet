#pragma once
#include "Entity.h"

namespace Violet {
	
	class VIOLET_API Script {
	public:
		virtual ~Script() = default;

		template<typename T>
		T& getComponent() {
			return m_entity.getComponent<T>();
		}

		template<typename T>
		bool hasComponent() {
			return m_entity.hasComponent<T>();
		}

	protected:
		virtual void onCreate() {}
		virtual void onUpdate(DeltaTime deltaTime) {}
		virtual void onDestroy() {} //TODO: To be called on destruction
	private:
		Entity m_entity; //Entity that the script will control
	private:
		friend class Scene;
	};

}
