#include "VIOPCH.h"
#include "Texture.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLTexture.h"
namespace Violet {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLTexture2D>(path);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	
	}

}