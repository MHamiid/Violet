#pragma once
#include "GraphicsAPI.h"

namespace Violet {

	class VIOLET_API RenderCommand {
	public:
		static void Init() { s_GraphicsAPI->init(); }
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_GraphicsAPI->setViewPort(x, y, width, height); }
		static void SetClearColor(const glm::vec4& color) { s_GraphicsAPI->setClearColor(color); }
		static void Clear() { s_GraphicsAPI->clear(); }
		static void DrawIndices(const Ref<VertexArray>& vertexArray, uint32_t numberOfIndicesToDraw = 0) { s_GraphicsAPI->drawIndices(vertexArray, numberOfIndicesToDraw); }  //If set to 0 draw the entire IndexBuffer
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) { s_GraphicsAPI->drawLines(vertexArray, vertexCount); }
		static void SetLineWidth(float width) { s_GraphicsAPI->setLineWidth(width); }
	private:
		static Scoped<GraphicsAPI> s_GraphicsAPI;  //Contains a pointer to the implementation class of GraphicsAPI interface

	};
}