#include "VIOPCH.h"
#include "RenderCommand.h"
#include "Violet/Platform/OpenGL/OpenGLRendererAPI.h"
namespace Violet {
	GraphicsAPI* RenderCommand::s_GraphicsAPI = new OpenGLRendererAPI();
}