#include "VIOPCH.h"
#include "Violet/Core/Input.h"
#include "Violet/Core/Application.h"
#include <GLFW/glfw3.h>

/*
* This file should be compiled only on windows platform.
* As that the linker would link the declarations in Input.h to the correct platform implementation.
*/

namespace Violet {

	bool Input::IsKeyPressed(const KeyCode keyCode)
	{
		GLFWwindow*  glfwWindow = static_cast<GLFWwindow*>(Application::GetApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer

		int state = glfwGetKey(glfwWindow, static_cast<int>(keyCode)); //Get the last reported state of a key

		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer

		int state = glfwGetMouseButton(glfwWindow, static_cast<int>(button)); //Get the last reported state of a mouse button

		return (state == GLFW_PRESS);
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer
		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{	
		return  GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return  GetMousePosition().y;
	}
	

}