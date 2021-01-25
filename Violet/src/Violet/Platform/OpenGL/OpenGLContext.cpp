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
		glfwMakeContextCurrent(m_windowHandle);

		//Loading Glad
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VIO_CORE_ASSERT(gladStatus, "Glad Initialization Failed!");

	}
	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}