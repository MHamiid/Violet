#include "VIOPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	struct QuadVertex {

		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinates;
		float textureIndex;
		float textureSizeFactor;

		/*Editor-Only*/
		int entityID;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuadsPerBatch = 10000;   //Batch 10000 quads together
		static const uint32_t MaxVerticesPerBatch = MaxQuadsPerBatch * 4;  // Times 4 cause we are drawing quads
		static const uint32_t MaxIndicesPerBatch = MaxQuadsPerBatch * 6;   // Times 6 cause 6 indices are required to draw a quad
		static const uint32_t MaxTextureSlots = 32;        //NOTE: Min value is 2, cause default white texture occupies the first slot,
														   //Setting MaxTextureSlots to 1 means we can't use any texture other than the default white texture,
														   //So that assertion is required in Init() that MaxTextureSlots >= 2
														   //TODO: Query the gpu driver to ask for the max texture slots

		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> defaultWhiteTexture;

		uint32_t indicesToBeDrawnCount = 0;    //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		QuadVertex* quadVertexBufferData = nullptr;
		QuadVertex* quadVertexBufferDataPtr = nullptr;  //A pointer into the buffer to manipulate and control the buffer

		std::array<Ref<Texture2D>, MaxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;  //Points to the first free space in the textureSlots array, starts from 1, 0 = default white texture

		glm::vec4 quadVertexPositions[4];   //Vec4 to match with the multiplication with the mat4 for tranformation later on

		/*STATISTICS*/
		static Renderer2D::SceneStatistics sceneStatistics;
	};
	
	static Renderer2DData* s_data; 
	Renderer2D::SceneStatistics Renderer2DData::sceneStatistics = Renderer2D::SceneStatistics(); //Intialize static member

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();

		//Assert There is atleast one slot texture can be used other than the default white texture
		VIO_CORE_ASSERT(Renderer2DData::MaxTextureSlots >= 2, "[Renderer2D] No Available Texture Slot");

		s_data->quadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(s_data->MaxVerticesPerBatch * sizeof(QuadVertex));

		quadVertexBuffer->setLayout({ {VertexAttributeDataType::Float3, "Position"           },
									  {VertexAttributeDataType::Float4, "Color"				 },
									  {VertexAttributeDataType::Float2, "TextureCoordinates" },
									  {VertexAttributeDataType::Float,  "TextureIndex"		 },
									  {VertexAttributeDataType::Float,  "TextureSizeFactor"	 },
									  {VertexAttributeDataType::Int,    "EntityID"			 } 
			});

		s_data->quadVertexArray->addVertexBufferAndLinkLayout(quadVertexBuffer);

		//Allocate enough storage for vertices
		s_data->quadVertexBufferData = new QuadVertex[s_data->MaxVerticesPerBatch];

		/*
		* NOTE: Use ref pointer instead and send it as a parameter to IndexBuffer::Create if we muliple threads are used to avoid deleting
		* quadIndices while it's being handled in IndexBuffer::Create on another thread
		*/
		uint32_t* quadIndices = new uint32_t[s_data->MaxIndicesPerBatch];

		uint32_t offset = 0;
		//Set data into quadIndices buffer
		for (uint32_t i = 0; i < s_data->MaxIndicesPerBatch; i+=6)  //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
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

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data->MaxIndicesPerBatch);
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

		//Set the positions for the initial quad where (0.0f, 0.0f) is the origin with size of (1.0f, 1.0f) going clock-wise
		s_data->quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f , 1.0f };  //Bottom_Left
		s_data->quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f , 1.0f };  //Bottom_Right
		s_data->quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f , 1.0f };  //Top_Right
		s_data->quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f , 1.0f };  //Top_Left
	}

	void Renderer2D::Shutdown()
	{
		delete [] s_data->quadVertexBufferData;
		//Free the memory and let VertexArray's destructor free gpu memory and other cleanups
		delete s_data;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getProjectionMatrix() * glm::inverse(transform)); //Set the uniform

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& editorCamera)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", editorCamera.getViewProjection()); //Set the uniform

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix()); //Set the uniform

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewBatch();
	}

	void Renderer2D::EndScene()
	{
		//If there is nothing to render return from function
		if (s_data->indicesToBeDrawnCount == 0) {
			return;
		}

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

		/*STATISTICS*/
		//Keep track of draw calls count for scene statistics
		s_data->sceneStatistics.drawCallsCount++;
	}
	/*
		QUADS
	*/
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		/*
		NOTE: Position points to the middle of the object
		*/
		glm::mat4 transfromationMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transfromationMatrix, color);
	}

	//Main DrawQuad function for colored quads
	void Renderer2D::DrawQuad(const glm::mat4& transfromationMatrix, const glm::vec4& color, int entityID)
	{
		/*
		* Quad vertices goes counter clock-wise.
		* In textureIndex parameter we bind the default white texture at index 0.0f to add no effect to the color (No Texture).
		*/

		//Check if the batch buffer is full
		if (IsBatchBufferFull()) {
			EndScene();
			StartNewBatch();
		}

		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec2 TextureCoordinates[4] = { {0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float TextureIndex = 0.0f; //Default white texture
		const float TextureSizeFactor = 1.0f; //Has no effect cuz we are not using a texture, only the default 1 pixel white texture, it is set to avoid warnings of unused uniform


		for (size_t i = 0; i < QuadVertexCount; i++) {
			AddVertexToBuffer(transfromationMatrix * s_data->quadVertexPositions[i], color, TextureCoordinates[i], TextureIndex, TextureSizeFactor, entityID);
		}

		s_data->indicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer

		/*STATISTICS*/
		//Keep track of quad count for scene statistics
		s_data->sceneStatistics.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureSizeFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		/*
		NOTE: Position points to the middle of the object
		*/
		glm::mat4 transfromationMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transfromationMatrix, texture, textureSizeFactor, tintColor);

	}
	//Main DrawQuad function for textured quads
	void Renderer2D::DrawQuad(const glm::mat4& transfromationMatrix, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor, int entityID)
	{
		/*
		* Quad vertices goes counter clock-wise.
		* Set the color to white to make no effect on the texture (No Color).
		*/

		//Check if the batch buffer is full
		if (IsBatchBufferFull()) {
			EndScene();
			StartNewBatch();
		}

		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec2 TextureCoordinates[4] = { {0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;    //Is sent to the shader to choose which texture slot to sample from

		//Search if the texture already exists in textureSlots array
		for (uint32_t i = 1; i < s_data->textureSlotIndex; i++) {

			if (*(s_data->textureSlots[i]) == *texture)  //If texture already in the array
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) //If texture not found in textureSlots array
		{
			if (IsTextureSlotsFull()) {
				EndScene();
				StartNewBatch();
			}
			textureIndex = float(s_data->textureSlotIndex);
			s_data->textureSlots[s_data->textureSlotIndex] = texture;
			s_data->textureSlotIndex++;

		}

		for (size_t i = 0; i < QuadVertexCount; i++) {
			AddVertexToBuffer(transfromationMatrix * s_data->quadVertexPositions[i], tintColor, TextureCoordinates[i], textureIndex, textureSizeFactor, entityID);
		}

		s_data->indicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		
		/*STATISTICS*/
		//Keep track of quad count for scene statistics
		s_data->sceneStatistics.quadCount++;
	}
	/*
		ROTATED QUADS
	*/
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationZ, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const glm::vec4& color)
	{
		/*
		NOTE: Position points to the middle of the object
		*/
		glm::mat4 transfromationMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), { 0.0f, 0.0f, 1.0f }) *   //TODO: Don't convert to radians if rotationZ already passed in radians (extra conversion processing)
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transfromationMatrix, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationZ, texture, textureSizeFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float textureSizeFactor, const glm::vec4& tintColor)
	{
		/*
		NOTE: Position points to the middle of the object
		*/
		glm::mat4 transfromationMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotationZ), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	
		DrawQuad(transfromationMatrix, texture, textureSizeFactor, tintColor);
	}

	void Renderer2D::DrawSprit(const glm::mat4& transformationMatrix, SpriteRendererComponent& spriteRendererComponent, int entityID)
	{
		DrawQuad(transformationMatrix, spriteRendererComponent.color, entityID);
	}

	void Renderer2D::StartNewBatch()
	{
		s_data->indicesToBeDrawnCount = 0;
		s_data->quadVertexBufferDataPtr = s_data->quadVertexBufferData;  //Let the pointer point to the begining of the buffer

		s_data->textureSlotIndex = 1;   //Resets to 1, cause index 0 is occupied by the default white texture
	}

	bool Renderer2D::IsBatchBufferFull()
	{
		/*
		* Returns true if the buffer is full
		*/
		if (s_data->indicesToBeDrawnCount >= Renderer2DData::MaxIndicesPerBatch) {
			return true;
		}
		return false;
	}

	bool Renderer2D::IsTextureSlotsFull()
	{
		/*
		* Returns true if the textureSlots array reached Renderer2D::MaxTextureSlots
		*/
		if (s_data->textureSlotIndex >= Renderer2DData::MaxTextureSlots) {
			return true;
		}
		return false;
	}

	void Renderer2D::AddVertexToBuffer(const glm::vec3& position, const glm::vec4& color,const glm::vec2& textureCoordinates, float textureIndex, float textureSizeFactor, int entityID)
	{
		//quadVertexBufferDataPtr points to the first empty space in the buffer
		s_data->quadVertexBufferDataPtr->position = position;
		s_data->quadVertexBufferDataPtr->color = color;
		s_data->quadVertexBufferDataPtr->textureCoordinates = textureCoordinates;
		s_data->quadVertexBufferDataPtr->textureIndex = textureIndex;
		s_data->quadVertexBufferDataPtr->textureSizeFactor = textureSizeFactor;
		s_data->quadVertexBufferDataPtr->entityID = entityID;
		s_data->quadVertexBufferDataPtr++; //Increment pointer to the next vertex
	}

	/*
	* Scene Statistics
	*/
	Renderer2D::SceneStatistics Renderer2D::GetSceneStatistics()
	{
		return Renderer2DData::sceneStatistics;
	}
	void Renderer2D::ResetSceneStatistics()
	{
		Renderer2DData::sceneStatistics.drawCallsCount = 0;
		Renderer2DData::sceneStatistics.quadCount = 0;
	}
}