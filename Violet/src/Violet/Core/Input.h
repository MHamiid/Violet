#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <glm/glm.hpp>
namespace Violet {
	
	class VIOLET_API Input {
	public:
		static bool IsKeyPressed(KeyCode keyCode);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
