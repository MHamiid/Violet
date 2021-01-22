#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Violet{

class VIOLET_API Layer {
public:
	Layer(const std::string& debugName);
	virtual ~Layer();

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate() {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& event) {}

	inline const std::string& getName() const { return m_debugName; }

protected:
	std::string m_debugName;

};

}
