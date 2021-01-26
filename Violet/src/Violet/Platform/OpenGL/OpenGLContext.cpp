#include "VIOPCH.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Violet {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle)
	{
		VIO_CORE_ASSERT(windowHandle, "Window Handle Is Null!");
	}
	void OpenGLContext::init()
	{	
		VIO_CORE_INFO("Creating OpenGL Context!");
		glfwMakeContextCurrent(m_windowHandle);

		//Loading Glad
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VIO_CORE_ASSERT(gladStatus, "Glad Initialization Failed!");

		//Log Graphics Card information
		VIO_CORE_INFO("Graphics Card Info:");
		VIO_CORE_INFO("		 Vendor: {0}",   glGetString(GL_VENDOR));
		VIO_CORE_INFO("		 Renderer: {0}", glGetString(GL_RENDERER));
		VIO_CORE_INFO("		 Version: {0}",  glGetString(GL_VERSION));

	}
	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}