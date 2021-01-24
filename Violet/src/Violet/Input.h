#pragma once

#include "Violet/Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
namespace Violet {
	
	/// <summary>
	/// Interface for inputs implementation 
	/// </summary>
	class VIOLET_API Input {
	public:
		static bool IsKeyPressed(Key keyCode) { return s_InputInstance->isKeyPressedIMPL(keyCode); }

		static bool IsMouseButtonPressed(Mouse button) { return s_InputInstance->isMouseButtonPressedIMPL(button); }
		static std::pair<float, float> GetMousePosition() { return s_InputInstance->getMousePositionIMPL(); }
		static float GetMouseX() { return s_InputInstance->getMouseXIMPL(); }
		static float GetMouseY() { return s_InputInstance->getMouseYIMPL(); }

	protected:
		virtual bool isKeyPressedIMPL(Key keyCode) = 0;

		virtual bool isMouseButtonPressedIMPL(Mouse button) = 0;
		virtual std::pair<float, float> getMousePositionIMPL() = 0;
		virtual float getMouseXIMPL() = 0;
		virtual float getMouseYIMPL() = 0;
		
	private:
		static Input* s_InputInstance; //To be set to a pointer of the implementation class

	
	};




}
