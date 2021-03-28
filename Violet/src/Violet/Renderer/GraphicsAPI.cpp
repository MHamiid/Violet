#include "VIOPCH.h"
#include "GraphicsAPI.h"
#include "Violet/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Violet {

	GraphicsAPI::API GraphicsAPI::s_GraphicsAPI = GraphicsAPI::API::OPENGL;   //Select which Graphics Api the application will use

	/*
	*  NOTE: Can't use VIO_CORE logging, cause the Create() function is called by a static in RenderCommand
	*        ====================> Scoped<GraphicsAPI> RenderCommand::s_GraphicsAPI = GraphicsAPI::Create(),
	*		 Which means that Violet::Log::init() is not called yet in the main function results in a program crash.
	*
	*  Therefore using program break without displaying an error message.
	*/

	Scoped<GraphicsAPI> GraphicsAPI::Create()
	{
		switch(s_GraphicsAPI)
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_BREAK;
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			return CreateScope<OpenGLRendererAPI>();
		}

		VIO_CORE_BREAK;
		return nullptr;
	}

}