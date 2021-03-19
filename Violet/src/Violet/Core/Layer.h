#pragma once

#include "Violet/Events/Event.h"
#include "DeltaTime.h"

namespace Violet{

class VIOLET_API Layer {
public:
	Layer(const std::string& debugName);
	virtual ~Layer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate(DeltaTime& deltaTime) {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& event) {}

	const std::string& getName() const { return m_debugName; }

protected:
	std::string m_debugName;

};

}
