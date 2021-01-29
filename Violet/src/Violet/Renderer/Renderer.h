#pragma once
#include "GraphicsAPI.h"
#include "RenderCommand.h"

namespace Violet {
	

	class VIOLET_API Renderer {
	public:
		static void BeginScene(); //TODO
		static void EndScene(); //TODO
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static GraphicsAPI::API GetCurrentGraphicsAPI() { return GraphicsAPI::getAPI(); }

	};

}