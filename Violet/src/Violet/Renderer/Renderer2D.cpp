#include "VIOPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	struct Renderer2DData {
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};
	
	static Renderer2DData* s_data; 

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();

		s_data->vertexArray = VertexArray::Create();



		float vertices[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		   -0.5f,  0.5f, 0.0f
		};


		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->setLayout({ {VertexAttributeDataType::Float3, "Position"} });

		s_data->vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_data->vertexArray->setIndexBuffer(indexBuffer);


		s_data->shader = Shader::Create("assets/shaders/SolidColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		//Free the memory and let vertex array free gpu memory
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->shader->bind(); 
		s_data->shader->setMat4("u_viewProjection", camera.getViewProjectionMatrix()); //Set the uniform
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_data->shader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
		s_data->shader->setFloat4("u_color", color);
		s_data->shader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position));

		s_data->vertexArray->bind();
		RenderCommand::DrawIndices(s_data->vertexArray);
	}

}