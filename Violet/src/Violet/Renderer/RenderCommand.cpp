#include "VIOPCH.h"
#include "RenderCommand.h"
#include "Violet/Platform/OpenGL/OpenGLRendererAPI.h"
namespace Violet {
	Scoped<GraphicsAPI> RenderCommand::s_GraphicsAPI = CreateScope<OpenGLRendererAPI>();
}