#include "VIOPCH.h"
#include "WindowsWindow.h"

namespace Violet {
	bool WindowsWindow::s_GLFWInitialized = false;

	Window* Window::Create(std::string title , unsigned int width , unsigned int height) {
		return new WindowsWindow(title, width, height);
	}


	WindowsWindow::WindowsWindow(std::string title, unsigned int width, unsigned int height) : Window(title,width,height)
	{
		init();
	}

	WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}

	bool WindowsWindow::isGLFWInit()
	{
		return s_GLFWInitialized;
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_VSync = enabled;

	}

	bool WindowsWindow::isVSyncEnabled() const
	{
		return m_VSync;
	}

	void WindowsWindow::init()
	{
		VIO_CORE_INFO("Creating Window {0} ({1} , {2})", m_title, m_width, m_height);
		//init ONLY ONCE PER PROGRAM GLFW if not initialized
		if (!s_GLFWInitialized) {
			int initialized = glfwInit(); //return 0 is failed
			VIO_CORE_ASSERT(initialized, "GLFW Initialization Failed!")
				s_GLFWInitialized = true;
		}
		m_window = glfwCreateWindow((int)m_width, (int)m_height, m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this); //Data to pass to Event callbacks.
		setVSync(true);

	}

	void WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_window);
	}

}