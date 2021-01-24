#include "VIOPCH.h"
#include "WindowsInput.h"

#include "Violet/Application.h"
#include <GLFW/glfw3.h>
namespace Violet {

	Input* WindowsInput::s_InputInstance = new WindowsInput();

	bool WindowsInput::isKeyPressedIMPL(Key keyCode)
	{
		GLFWwindow*  glfwWindow = static_cast<GLFWwindow*>(Application::getApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer

		int state = glfwGetKey(glfwWindow, (int)keyCode); //Get the last reported state of a key

		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool WindowsInput::isMouseButtonPressedIMPL(Mouse button)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::getApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer

		int state = glfwGetMouseButton(glfwWindow, (int)button); //Get the last reported state of a mouse button

		return (state == GLFW_PRESS);
	}

	std::pair<float, float> WindowsInput::getMousePositionIMPL()
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::getApplicationInstance().getWindow().getNativeWindow()); //Get out window pointer
		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);
		return std::pair<float, float>((float)xPos , (float)yPos);
	}

	float WindowsInput::getMouseXIMPL()
	{	
		auto [x, y] = getMousePositionIMPL();
		return x;
	}

	float WindowsInput::getMouseYIMPL()
	{
		auto [x, y] = getMousePositionIMPL();
		return y;
	}
	

}