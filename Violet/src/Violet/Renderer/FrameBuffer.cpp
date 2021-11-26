#include "VIOPCH.h"
#include "FrameBuffer.h"
#include "Violet/Renderer/Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Violet {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			//VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLFrameBuffer>(specification);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}

}