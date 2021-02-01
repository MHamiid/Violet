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
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transformation)
	{
		//Setup the shader and camera uniform
		shader->bind();
		shader->setMat4("u_viewProjection", s_SceneData->viewProjectionMatrix); //Set the uniform
		shader->setMat4("u_transformation", transformation);


		vertexArray->bind();
		RenderCommand::DrawIndices(vertexArray);
	}
}