#pragma once
#include "Violet/Renderer/GraphicsAPI.h"

namespace Violet {

	class VIOLET_API OpenGLRendererAPI : public GraphicsAPI {
	public:
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndices(const std::shared_ptr<VertexArray>& vertexArray) override;
	};

}