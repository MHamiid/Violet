#include "VIOPCH.h"
#include "WindowsWindow.h"

namespace Violet {
	bool WindowsWindow::s_GLFWInitialized = false;

	Window* Window::Create(const WindowProperties& props) {
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProperties& props) : Window(props)
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
		m_properties.m_VSync = enabled;

	}

	bool WindowsWindow::isVSyncEnabled() const
	{
		return m_properties.m_VSync;
	}

	void WindowsWindow::init()
	{
		VIO_CORE_INFO("Creating Window {0} ({1} , {2})", m_properties.m_title, m_properties.m_width, m_properties.m_height);
		//init ONLY ONCE PER PROGRAM GLFW if not initialized
		if (!s_GLFWInitialized) {
			int initialized = glfwInit(); //return 0 is failed
			VIO_CORE_ASSERT(initialized, "GLFW Initialization Failed!")
				s_GLFWInitialized = true;
		}
		m_window = glfwCreateWindow((int)m_properties.m_width, (int)m_properties.m_height, m_properties.m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_properties); //Data to pass to Event callbacks.
		//setVSync(true);


		////Getting GLFW events(setting callbacks) and link it to Violet events.
		//glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		//	WindowProperties* wind = (WindowProperties*)glfwGetWindowUserPointer(window);
		//	wind->m_width = width;
		//	
		//	wind->m_height = height;
		//	VIO_CORE_TRACE("GLFW Window Resized: {0} , {1}", width, height);
		//	
		//});

	}

	void WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_window);
	}

}