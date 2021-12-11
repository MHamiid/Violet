#pragma once
#include "Violet/Renderer/GraphicsAPI.h"

namespace Violet {

	class VIOLET_API OpenGLRendererAPI : public GraphicsAPI {
	public:
		virtual void init() override;
		virtual void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndices(const Ref<VertexArray>& vertexArray, uint32_t numberOfIndicesToDraw = 0) override;
		virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void setLineWidth(float width) override;
	};

}