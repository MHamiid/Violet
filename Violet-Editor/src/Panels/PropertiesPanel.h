#pragma once
#include "Violet/Core/Core.h"
#include "Violet/Core/Log.h"
#include "Violet/Scene/Entity.h"

namespace Violet {

	class PropertiesPanel {
	public:
		PropertiesPanel() = default;
		PropertiesPanel(Entity entity);

		void setEntityContext(Entity entity);

		void onImGuiRender();

	private:
		void drawComponents(Entity entity);
		void drawAddComponentButton();
	private:
		template<typename ComponentType, typename UIFunction>
		static void drawComponent(const std::string& name, Entity entity, UIFunction UIFUNC);
	private:
		Entity m_entityContext;
	};

}