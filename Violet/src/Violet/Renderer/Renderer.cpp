#include "VIOPCH.h"
#include "Renderer.h"

namespace Violet {

	//Initialize static member
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//TODO
		s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	void Renderer::EndScene()	
	{
		//TODO
	}
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		//Setup the shader and camera uniform
		shader->bind();
		shader->setMat4("u_viewProjection", s_SceneData->viewProjectionMatrix); //Set the uniform


		vertexArray->bind();
		RenderCommand::DrawIndices(vertexArray);
	}
}