#include "VIOPCH.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Violet {

	//Initialize static member
	Scoped<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

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