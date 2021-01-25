#pragma once
#include "Violet/Renderer/GraphicsContext.h"
struct GLFWwindow;

namespace Violet {

	class VIOLET_API OpenGLContext: public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* m_windowHandle;

	};

}