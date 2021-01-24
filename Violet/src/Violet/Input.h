#pragma once

#include "Violet/Core.h"

namespace Violet {
	
	/// <summary>
	/// Interface for inputs implementation 
	/// </summary>
	class VIOLET_API Input {
	public:
		static bool IsKeyPressed(int keyCode) { return s_InputInstance->isKeyPressedIMPL(keyCode); }

		static bool IsMouseButtonPressed(int button) { return s_InputInstance->isMouseButtonPressedIMPL(button); }
		static std::pair<float, float> GetMousePosition() { return s_InputInstance->getMousePositionIMPL(); }
		static float GetMouseX() { return s_InputInstance->getMouseXIMPL(); }
		static float GetMouseY() { return s_InputInstance->getMouseYIMPL(); }

	protected:
		virtual bool isKeyPressedIMPL(int keyCode) = 0;

		virtual bool isMouseButtonPressedIMPL(int button) = 0;
		virtual std::pair<float, float> getMousePositionIMPL() = 0;
		virtual float getMouseXIMPL() = 0;
		virtual float getMouseYIMPL() = 0;
		
	private:
		static Input* s_InputInstance; //To be set to a pointer of the implementation class

	
	};




}
