#include "VIOPCH.h"
#include "Renderer.h"

namespace Violet {
	void Renderer::BeginScene()
	{
		//TODO
	}
	void Renderer::EndScene()	
	{
		//TODO
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		RenderCommand::DrawIndices(vertexArray);
	}
}