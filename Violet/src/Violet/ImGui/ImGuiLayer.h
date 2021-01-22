#pragma once
#include "Violet/Layer.h"
namespace Violet {
	
	class VIOLET_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event& event) override;

	};


}