#include "VIOPCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLShader.h"
namespace Violet {
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
		switch (Renderer::GetCurrentRendererApi())
		{
		case RendererAPI::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case RendererAPI::OPENGL:
			VIO_CORE_INFO("[Renderer] OpenGL Selected");
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
    }
}
