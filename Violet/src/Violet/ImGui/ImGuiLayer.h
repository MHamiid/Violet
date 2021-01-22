#pragma once
#include "Violet/Layer.h"
#include "Violet/Window.h"
namespace Violet {
	
	class VIOLET_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& event) override;
	public:
		void beginImGuiFrame();
		void endImGuiFrame();
	};


}