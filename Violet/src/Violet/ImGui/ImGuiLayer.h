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
		virtual void onEvent(Event& event) override;
	public:
		static void BeginImGuiFrame();
		static void EndImGuiFrame();
		static void SetVioletThemeColors();
		static void SetGoldThemeColors();
		static void SetDarkThemeColors();
		static void SetDarkRedThemeColors();
	public:
		void setImGuiToBlockEvents(bool block) { m_blockImGuiEvents = block; }
	private:
		bool m_blockImGuiEvents = true;
		static ImGuiLayer* s_ImGuiLayer;
	};


}