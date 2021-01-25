#pragma once
#include "Violet/Core.h"

namespace Violet {
	
	/*
		Interface
	*/
	class VIOLET_API GraphicsContext {
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};


}