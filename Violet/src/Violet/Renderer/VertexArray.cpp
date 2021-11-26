#include "VIOPCH.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLVertexArray.h"
namespace Violet {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			//VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLVertexArray>();
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}
}