#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <glm/glm.hpp>
namespace Violet {
	
	/// <summary>
	/// Interface for inputs implementation 
	/// </summary>
	class VIOLET_API Input {
	public:
		static bool IsKeyPressed(Key keyCode);

		static bool IsMouseButtonPressed(Mouse button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};




}
