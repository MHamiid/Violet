#pragma once
#include "GraphicsAPI.h"

namespace Violet {

	class VIOLET_API RenderCommand {
	public:
		static void SetClearColor(const glm::vec4& color) { s_GraphicsAPI->setClearColor(color); };
		static void Clear() { s_GraphicsAPI->clear(); }
		static void DrawIndices(const Ref<VertexArray>& vertexArray) { s_GraphicsAPI->drawIndices(vertexArray); }
	private:
		static GraphicsAPI* s_GraphicsAPI;  //Contains a pointer to the implementation class of GraphicsAPI interface

	};
}