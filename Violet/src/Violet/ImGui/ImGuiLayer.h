#pragma once
#include "Violet/Core/Layer.h"
#include "Violet/Core/Window.h"
namespace Violet {
	
	class VIOLET_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& event) override;
	public:
		static void BeginImGuiFrame();
		static void EndImGuiFrame();
	private:
		static ImGuiLayer* s_ImGuiLayer;
	};


}