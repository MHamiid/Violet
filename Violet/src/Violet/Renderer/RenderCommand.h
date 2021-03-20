#pragma once
#include "GraphicsAPI.h"

namespace Violet {

	class VIOLET_API RenderCommand {
	public:
		static void Init() { s_GraphicsAPI->init(); }
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_GraphicsAPI->setViewPort(x, y, width, height); }
		static void SetClearColor(const glm::vec4& color) { s_GraphicsAPI->setClearColor(color); }
		static void Clear() { s_GraphicsAPI->clear(); }
		static void DrawIndices(const Ref<VertexArray>& vertexArray) { s_GraphicsAPI->drawIndices(vertexArray); }
	private:
		static Scoped<GraphicsAPI> s_GraphicsAPI;  //Contains a pointer to the implementation class of GraphicsAPI interface

	};
}