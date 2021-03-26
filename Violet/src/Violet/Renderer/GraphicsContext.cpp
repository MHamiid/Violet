#include "VIOPCH.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLContext.h"

namespace Violet {
	Scoped<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}
}