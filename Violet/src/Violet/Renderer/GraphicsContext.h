#pragma once

namespace Violet {
	
	/*
		Interface
	*/
	class VIOLET_API GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};


}