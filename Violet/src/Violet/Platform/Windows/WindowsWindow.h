#pragma once

#include "Violet/Window.h"
#include <GLFW/glfw3.h>
namespace Violet {

	class VIOLET_API WindowsWindow : public Window {

	public:
		WindowsWindow(std::string title, unsigned int width, unsigned int height);
		virtual ~WindowsWindow();

		static bool isGLFWInit();

	public:
		//Overriding Pure Virtual funcitons
		virtual void onUpdate() override;
		inline unsigned int getWidth() const override { return m_width; }	
		virtual unsigned int getHeight() const override { return m_height; }

		virtual void setVSync(bool enabled) override;
		virtual bool isVSyncEnabled() const override;
		
	private:
		//Overriding Pure Virtual funcitons
		virtual void init() override;
		virtual void shutDown() override;
	private:
		GLFWwindow* m_window;
		static bool s_GLFWInitialized;
	};





}


