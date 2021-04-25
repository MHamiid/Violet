#include "VIOPCH.h"
#include "WindowsWindow.h"
#include "Violet/Events/ApplicationEvent.h"
#include "Violet/Events/KeyEvent.h"
#include "Violet/Events/MouseEvent.h"
#include "Violet/Renderer/GraphicsContext.h"
#include "Violet/Renderer/Renderer.h"
namespace Violet {
	
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		VIO_CORE_ERROR("GLFW Error ({0}): {1}", error , description);
	}
	Scoped<Window> Window::Create(const WindowProperties& props) {
		return CreateScope<WindowsWindow>(props);
	}


	WindowsWindow::WindowsWindow(const WindowProperties& props) : Window(props)
	{
		init();
	}

	WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		m_context->swapBuffers();
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
		VIO_CORE_INFO("Creating Window: {0} ({1} , {2})!", m_properties.m_title, m_properties.m_width, m_properties.m_height);
		//init ONLY ONCE PER PROGRAM GLFW if not initialized
		if (!s_GLFWInitialized) {
			int initialized = glfwInit(); //return 0 is failed
			VIO_CORE_ASSERT(initialized, "GLFW Initialization Failed!");
			//If initialized successfully set GLFW error call back function
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		m_window = glfwCreateWindow((int)m_properties.m_width, (int)m_properties.m_height, m_properties.m_title.c_str(), nullptr, nullptr);
		
		//Setting up Graphics context for the window
		m_context = GraphicsContext::Create(m_window);
		m_context->init();

#ifdef VIO_DEBUG_MODE
		if (Renderer::GetCurrentGraphicsAPI() == GraphicsAPI::API::OPENGL) {
			VIO_CORE_INFO("OpenGL Debug Messages Enabled");
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		}
#endif

		glfwSetWindowUserPointer(m_window, &m_properties); //Data to pass to Event callbacks.
		setVSync(m_properties.m_VSync);


		//Getting GLFW events(setting callbacks) and link it to Violet events.
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* GlfwWindow, int width, int height){
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Update Our window properties
			windowProps->m_width = width;
			windowProps->m_height = height;

			//Set the appropriate event
			WindowResizeEvent event(width, height);

			//Call the Event Callback function with the event
			windowProps->m_FUNCEventCallback(event);
			
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* GlfwWindow) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			WindowCloseEvent event;

			//Call the Event Callback function with the event
			windowProps->m_FUNCEventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* GlfwWindow, int key , int scanCode , int action , int modes) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				//Call the Event Callback function with the event
				windowProps->m_FUNCEventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				windowProps->m_FUNCEventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				windowProps->m_FUNCEventCallback(event);

				break;
			}
			}

			
		});


		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* GlfwWindow, int key, int action, int modes) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(key);
				//Call the Event Callback function with the event
				windowProps->m_FUNCEventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(key);
				windowProps->m_FUNCEventCallback(event);
				break;
			}
			}


		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* GlfwWindow, double x , double y) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			MouseMovedEvent event((float)x,(float)y);
			
			//Call the Event Callback function with the event
			windowProps->m_FUNCEventCallback(event);
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* GlfwWindow, double xOffset, double yOffset) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
		
			//Call the Event Callback function with the event
			windowProps->m_FUNCEventCallback(event);
		});

		glfwSetDropCallback(m_window, [](GLFWwindow* GlfwWindow, int count, const char** paths) {
			//Getting the data pointer that was set to be passed by glfwSetWindowUserPointer()
			WindowProperties* windowProps = (WindowProperties*)glfwGetWindowUserPointer(GlfwWindow);

			//Set the appropriate event
			ItemsDroppedEvent event(count, paths);

			//Call the Event Callback function with the event
			windowProps->m_FUNCEventCallback(event);
		});

	}

	void WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_window);
	}

}