#include "VIOPCH.h"
#include "RenderCommand.h"
namespace Violet {
	Scoped<GraphicsAPI> RenderCommand::s_GraphicsAPI = GraphicsAPI::Create();    //Creates based on which Graphics API is selected in s_GraphicsAPI
}