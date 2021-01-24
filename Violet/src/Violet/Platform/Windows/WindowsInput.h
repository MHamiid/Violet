#pragma once
#include "Violet/Input.h"

namespace Violet {

	class WindowsInput : public Input {
	public:
		WindowsInput() {}
		~WindowsInput() {}
	protected:
		virtual bool isKeyPressedIMPL(Key keyCode) override;

		virtual bool isMouseButtonPressedIMPL(Mouse button) override;
		virtual std::pair<float, float> getMousePositionIMPL() override;
		virtual float getMouseXIMPL() override;
		virtual float getMouseYIMPL() override;
		

	};
}	