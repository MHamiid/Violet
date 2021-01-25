#pragma once

#include "Violet/Window.h"
#include "Violet/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>
namespace Violet {

	class VIOLET_API WindowsWindow : public Window {

	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

	public:
		//Overriding Pure Virtual funcitons
		virtual void onUpdate() override;
		virtual unsigned int getWidth() const override { return m_properties.m_width; }	
		virtual unsigned int getHeight() const override { return m_properties.m_height; }
		virtual void setVSync(bool enabled) override;
		virtual bool isVSyncEnabled() const override;

		virtual void* getNativeWindow() const override { return m_window; }
	protected:
		//Overriding Pure Virtual funcitons
		virtual void init() override;
		virtual void shutDown() override;
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;
	};





}


