#include "VIOPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	struct QuadVertex {
		//TODO: Add textureID
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinates;
		float textureIndex;
		float textureSizeFactor;
	};

	struct Renderer2DData {
		const uint32_t MaxObjectsPerDraw = 10000;   //Batch 10000 object together
		const uint32_t MaxVerticesPerDraw = MaxObjectsPerDraw * 4;  // Times 4 cause we are drawing quads
		const uint32_t MaxIndicesPerDraw = MaxObjectsPerDraw * 6;   // Times 6 cause 6 indices are required to draw a quad
		static const uint32_t MaxTextureSlots = 32;        //TODO: Query the gpu driver to ask for the max texture slots

		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> defaultWhiteTexture;

		uint32_t indicesToBeDrawnCount = 0;    //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		QuadVertex* quadVertexBufferData = nullptr;
		QuadVertex* quadVertexBufferDataPtr = nullptr;  //A pointer into the buffer to manipulate and control the buffer

		std::array<Ref<Texture2D>, MaxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;  //Points to the first free space in the textureSlots array, starts from 1, 0 = default white texture
	};
	
	static Renderer2DData* s_data; 

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();

		s_data->quadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(s_data->MaxVerticesPerDraw * sizeof(QuadVertex));

		quadVertexBuffer->setLayout({ {VertexAttributeDataType::Float3, "Position"},
									{VertexAttributeDataType::Float4, "Color"},
									{VertexAttributeDataType::Float2, "TextureCoordinates"},
									{VertexAttributeDataType::Float, "TextureIndex"},
									{VertexAttributeDataType::Float, "TextureSizeFactor"} });

		s_data->quadVertexArray->addVertexBufferAndLinkLayout(quadVertexBuffer);

		//Allocate enough storage for vertices
		s_data->quadVertexBufferData = new QuadVertex[s_data->MaxVerticesPerDraw];

		/*
		*NOTE: Use ref pointer instead and send it as a parameter to IndexBuffer::Create if we muliple threads are used to avoid deleting
		* quadIndices while it's being handled in IndexBuffer::Create on another thread
		*/
		uint32_t* quadIndices = new uint32_t[s_data->MaxIndicesPerDraw];

		uint32_t offset = 0;
		//Set data into quadIndices buffer
		for (uint32_t i = 0; i < s_data->MaxIndicesPerDraw; i+=6)  //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		{  
			//First triangle in the quad
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			//Second triangle in the quad
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data->MaxIndicesPerDraw);
		s_data->quadVertexArray->setIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;


		s_data->defaultWhiteTexture = Texture2D::Create(1, 1);  //Create a one pixel texture
		uint32_t whiteTextureData = 0xffffffff;   //4-Bytes
		s_data->defaultWhiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		s_data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_data->textureShader->bind();

		/*Set The Texture Samplers Array*/
		int32_t textureSamplers[s_data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_data->MaxTextureSlots; i++) {
			textureSamplers[i] = i; 
		}
		s_data->textureShader->setIntArray("u_textures", textureSamplers, s_data->MaxTextureSlots);


		s_data->textureSlots[0] = s_data->defaultWhiteTexture;    //Set the first element to our default white texture (no texture used and only the color is taken in the shader)

	}

	void Renderer2D::Shutdown()
	{
		delete [] s_data->quadVertexBufferData;
		//Free the memory and let vertex array free gpu memory
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix()); //Set the uniform

		/*Reset Scene*/
		s_data->indicesToBeDrawnCount = 0;
		s_data->quadVertexBufferDataPtr = s_data->quadVertexBufferData;  //Let the pointer point to the begining of the buffer

		s_data->textureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_data->quadVertexBufferDataPtr - (uint8_t*)s_data->quadVertexBufferData);  //NOTE: Converted to uint8_t (1 Byte) to get the size and not the number of vertcies
		//Upload vertex buffer to GPU
		s_data->quadVertexArray->getVertexBuffers()[0]->setData(s_data->quadVertexBufferData, dataSize);

		Flush();
	}
	/*
	* Sends the draw call for the batch
	*/
	void Renderer2D::Flush()
	{
		//VIO_CORE_DEBUG("[Renderer2D] Issuing A Draw Call For Batch Of Size {0}", s_data->indicesToBeDrawnCount / 6); //Each quad contains 6 indices
		//VIO_CORE_DEBUG("[Renderer2D] Issuing A Draw Call With {0} Textures Bound", s_data->textureSlotIndex);
		/*Bind All The Textures*/
		for (uint32_t i = 0; i < s_data->textureSlotIndex; i++) 
		{
			s_data->textureSlots[i]->bind(i);
		}


		s_data->quadVertexArray->bind();
		RenderCommand::DrawIndices(s_data->quadVertexArray, s_data->indicesToBeDrawnCount);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	/*
		QUADS
	*/
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		/*
		* NOTE: position points to the middle of the object.
		* Quad vertices goes counter clock-wise.
		* In textureIndex parameter we bind the default white texture at index 0.0f to add no effect to the color (No Texture)
		*/
		const float textureIndex = 0.0f; //Default white texture
		const float textureSizeFactor = 1.0f; //Has no effect cuz we are not using a texture, only the default 1 pixel white texture, it is set to avoid warnings of unused uniform

		//Bottom-Left
		AddVertexToBuffer({ position.x - (size.x / 2.0f), position.y - (size.y / 2.0f), position.z }, color,  glm::vec2(0.0f, 0.0f), textureIndex, textureSizeFactor);
		//Bottom-Right
		AddVertexToBuffer({ position.x + (size.x / 2.0f), position.y - (size.y / 2.0f), position.z }, color, glm::vec2(1.0f, 0.0f), textureIndex, textureSizeFactor);
		//Top-Right
		AddVertexToBuffer({ position.x + (size.x / 2.0f), position.y + (size.y / 2.0f), position.z }, color, glm::vec2(1.0f, 1.0f), textureIndex, textureSizeFactor);
		//Top-Left
		AddVertexToBuffer({ position.x - (size.x / 2.0f), position.y + (size.y / 2.0f), position.z }, color, glm::vec2(0.0f, 1.0f), textureIndex, textureSizeFactor);

		s_data->indicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer

		//s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
		//s_data->textureShader->setFloat4("u_color", color);
		//s_data->textureShader->setFloat("u_sizeFactor", 1.0f);  //Has no effect cuz we are not using a texture, only the default 1 pixel white texture, it is set to avoid warnings of unused uniform
		//s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
		//	glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }));

		////Bind white default texture to add no effect to the color (No Texture)
		//s_data->defaultWhiteTexture->bind(0);

		//s_data->quadVertexArray->bind();
		//RenderCommand::DrawIndices(s_data->quadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureSizeFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		/*
		* NOTE: Set the color to white to make no effect on the texture (No Color)
		*/
		
		float textureIndex = 0.0f;    //Is sent to the shader to choose which texture slot to sample from
		
		//Search if the texture already exists in textureSlots array
		for (uint32_t i = 1; i < s_data->textureSlotIndex; i++) {
		
			if(*(s_data->textureSlots[i]) == *texture)  //If texture already in the array
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) //If texture not found in textureSlots array
		{
			//TODO: If textureSlotIndex exceeded the MaxTextureSlots end and flush the scene
			textureIndex = float(s_data->textureSlotIndex);
			s_data->textureSlots[s_data->textureSlotIndex] = texture;
			s_data->textureSlotIndex++;
		
		}

		/*
		* NOTE: position points to the middle of the object.
		* Quad vertices goes counter clock-wise.
		*/
		//Bottom-Left
		AddVertexToBuffer({ position.x - (size.x / 2.0f), position.y - (size.y / 2.0f), position.z }, tintColor, glm::vec2(0.0f, 0.0f), textureIndex, textureSizeFactor);
		//Bottom-Right
		AddVertexToBuffer({ position.x + (size.x / 2.0f), position.y - (size.y / 2.0f), position.z }, tintColor, glm::vec2(1.0f, 0.0f), textureIndex, textureSizeFactor);
		//Top-Right
		AddVertexToBuffer({ position.x + (size.x / 2.0f), position.y + (size.y / 2.0f), position.z }, tintColor, glm::vec2(1.0f, 1.0f), textureIndex, textureSizeFactor);
		//Top-Left
		AddVertexToBuffer({ position.x - (size.x / 2.0f), position.y + (size.y / 2.0f), position.z }, tintColor, glm::vec2(0.0f, 1.0f), textureIndex, textureSizeFactor);

		s_data->indicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer

		//s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
		//s_data->textureShader->setFloat4("u_color", tintColor);
		//s_data->textureShader->setFloat("u_sizeFactor", sizeFactor);
		//s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
		//	glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }));

		//texture->bind(0);

		//s_data->quadVertexArray->bind();
		//RenderCommand::DrawIndices(s_data->quadVertexArray);
	}
	/*
	//	ROTATED QUADS
	//*/
	//void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	//{
	//	DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationZ, color);
	//}

	//void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	//{
	//	s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
	//	s_data->textureShader->setFloat4("u_color", color);
	//	s_data->textureShader->setFloat("u_sizeFactor", 1.0f);  //Has no effect cuz we are not using a texture, only the default 1 pixel white texture, it is set to avoid warnings of unused uniform
	//	s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
	//												glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), glm::vec3(0, 0, 1)) *
	//												glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}));

	//	//Bind white default texture to add no effect to the color (No Texture)
	//	s_data->defaultWhiteTexture->bind(0);

	//	s_data->quadVertexArray->bind();
	//	RenderCommand::DrawIndices(s_data->quadVertexArray);
	//}

	//void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float sizeFactor, const glm::vec4& tintColor)
	//{
	//	DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationZ, texture, sizeFactor, tintColor);
	//}

	//void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float sizeFactor, const glm::vec4& tintColor)
	//{
	//	/*
	//	* NOTE: Set the color to white to make no effect on the texture (No Color)
	//	*/
	//	s_data->textureShader->bind(); //Make sure that the shader is bound, TODO: We may keep track of the current bound shader and in bind() function we ignore the call if the shader is already bound.
	//	s_data->textureShader->setFloat4("u_color", tintColor);
	//	s_data->textureShader->setFloat("u_sizeFactor", sizeFactor);
	//	s_data->textureShader->setMat4("u_transformation", glm::translate(glm::mat4(1.0f), position) *
	//												glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), glm::vec3(0, 0, 1)) *
	//												glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }));

	//	texture->bind(0);

	//	s_data->quadVertexArray->bind();
	//	RenderCommand::DrawIndices(s_data->quadVertexArray);
	//}

	void Renderer2D::AddVertexToBuffer(const glm::vec3& position, const glm::vec4& color, glm::vec2& textureCoordinates, float textureIndex, float textureSizeFactor)
	{
		//quadVertexBufferDataPtr points to the first empty space in the buffer
		s_data->quadVertexBufferDataPtr->position = position;
		s_data->quadVertexBufferDataPtr->color = color;
		s_data->quadVertexBufferDataPtr->textureCoordinates = textureCoordinates;
		s_data->quadVertexBufferDataPtr->textureIndex = textureIndex;
		s_data->quadVertexBufferDataPtr->textureSizeFactor = textureSizeFactor;
		s_data->quadVertexBufferDataPtr++; //Increment pointer to the next vertex
	}


}