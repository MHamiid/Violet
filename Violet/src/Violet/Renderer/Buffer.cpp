#include "VIOPCH.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLBuffer.h"
namespace Violet {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return new OpenGLVertexBuffer(vertices, size);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch(Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return new OpenGLIndexBuffer(indices, count);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}
}