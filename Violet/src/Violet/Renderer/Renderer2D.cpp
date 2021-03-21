#include "VIOPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	struct Renderer2DData {
		Ref<VertexArray> vertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> defaultWhiteTexture;
	};
	
	static Renderer2DData* s_data; 

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();

		s_data->vertexArray = VertexArray::Create();



		float vertices[] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};


		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->setLayout({ {VertexAttributeDataType::Float3, "Position"},
								  {VertexAttributeDataType::Float2, "TextureCoordinates"} });

		s_data->vertexArray->addVertexBufferAndLinkLayout(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_data->vertexArray->setIndexBuffer(indexBuffer);

		s_data->defaultWhiteTexture = Texture2D::Create(1, 1);  //Create a one pixel texture
		uint32_t whiteTextureData = 0xffffffff;   //4-Bytes
		s_data->defaultWhiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		s_data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_data->textureShader->bind();
		s_data->textureShader->setInt("u_texture", 0); //the texture slot what the texture sampler gonna sample from
	}

	void Renderer2D::Shutdown()
	{
		//Free the memory and let vertex array free gpu memory
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix()); //Set the uniform
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotationZ, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	{
		s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
		s_data->textureShader->setFloat4("u_color", color);
		s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
													glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), glm::vec3(0, 0, 1)) *
													glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}));

		//Bind white default texture to add no effect to the color (No Texture)
		s_data->defaultWhiteTexture->bind(0);

		s_data->vertexArray->bind();
		RenderCommand::DrawIndices(s_data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotationZ, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		/*
		* NOTE: Set the color to white to make no effect on the texture (No Color)
		*/
		s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
		s_data->textureShader->setFloat4("u_color", color); 
		s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
													glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), glm::vec3(0, 0, 1)) *
													glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }));

		texture->bind(0);

		s_data->vertexArray->bind();
		RenderCommand::DrawIndices(s_data->vertexArray);
	}


}