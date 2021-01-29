#include "VIOPCH.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLVertexArray.h"
namespace Violet {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetCurrentRendererApi())
		{
		case RendererAPI::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case RendererAPI::OPENGL:
			VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return new OpenGLVertexArray();
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}
}