#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
namespace Violet {
	
	/// <summary>
	/// Interface for inputs implementation 
	/// </summary>
	class VIOLET_API Input {
	public:
		//virtual ~Input() = default; NOTE: No need to have a virtual destructor, it would be a singleton anyway stored in a static member
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		static bool IsKeyPressed(Key keyCode) { return s_InputInstance->isKeyPressedIMPL(keyCode); }

		static bool IsMouseButtonPressed(Mouse button) { return s_InputInstance->isMouseButtonPressedIMPL(button); }
		static std::pair<float, float> GetMousePosition() { return s_InputInstance->getMousePositionIMPL(); }
		static float GetMouseX() { return s_InputInstance->getMouseXIMPL(); }
		static float GetMouseY() { return s_InputInstance->getMouseYIMPL(); }

	protected:
		Input() = default;

		virtual bool isKeyPressedIMPL(Key keyCode) = 0;

		virtual bool isMouseButtonPressedIMPL(Mouse button) = 0;
		virtual std::pair<float, float> getMousePositionIMPL() = 0;
		virtual float getMouseXIMPL() = 0;
		virtual float getMouseYIMPL() = 0;
		
	private:
		static Scoped<Input> s_InputInstance; //To be set to a pointer of the implementation class

	
	};




}
