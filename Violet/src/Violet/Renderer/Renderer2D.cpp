#include "VIOPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	struct QuadVertex {
		/*
		* position: Position in world space with the transformation matrix applied
		* (We are using batch rendering, which means all the trasformation 
		* calculations are done on the CPU side before being sent to the Shader)
		*/
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinates;
		float textureIndex;
		float textureSizeFactor;

		/*Editor-Only*/
		int entityID;
	};

	/*
	* NOTE: Drawing a circle is essentially drawing a quad with a different shader, and it has a different vertex layout
	*/
	struct CircleVertex {
		/*
		* worldPosition: Position in world space with the transformation matrix applied
		* (We are using batch rendering, which means all the trasformation
		* calculations are done on the CPU side before being sent to the Shader)
		* 
		* localPosition: Is used for creating a custom range [-1.0f to 1.0f] on XY axes
		* , which is used when drawing the circle in CircleShader.
		*/
		glm::vec3 worldPosition;
		glm::vec3 localPosition;    //TODO: Probably make it vec2, as there is no need for the Z axis
		glm::vec4 color;
		float Thickness;
		float Fade;

		/*Editor-Only*/
		int entityID;
	};

	struct LineVertex {
		/*
		* position: Position in world space with the transformation matrix applied
		* (We are using batch rendering, which means all the trasformation
		* calculations are done on the CPU side before being sent to the Shader)
		*/
		glm::vec3 position;
		glm::vec4 color;

		/*
		 * NOTE: entityID currently has no usage (lines are only used for visualizing physics colliders)
		 * , unless Lines are used as in game entity and is included in the ECS(Entity Component System)
		 */
		/*Editor-Only*/
		int entityID;
	};

	struct Renderer2DData {
		/*
		* TODO: Make MaxQuadsPerBatch resizable, instead of allocating large space of memory 
		* for the quads that may not be used. Maybe start off with something small like 100
		* and double/+50% the size whenever more memory is needed.
		* NOTE: The constants below are also used for circles.
		* NOTE: The constants below are also used for Lines
		* (should be refactored, lines are only used for visualizing physics collider
		* , we don't need that many lines).
		*/
		static const uint32_t MaxQuadsPerBatch = 10000;   //Batch 10000 quads together
		static const uint32_t MaxVerticesPerBatch = MaxQuadsPerBatch * 4;  // Times 4 cause we are drawing quads
		static const uint32_t MaxIndicesPerBatch = MaxQuadsPerBatch * 6;   // Times 6 cause 6 indices are required to draw a quad
		static const uint32_t MaxTextureSlots = 32;        //NOTE: Min value is 2, cause default white texture occupies the first slot,
														   //Setting MaxTextureSlots to 1 means we can't use any texture other than the default white texture,
														   //So that assertion is required in Init() that MaxTextureSlots >= 2
														   //TODO: Query the gpu driver to ask for the max texture slots
		/*Quad*/
		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;
		Ref<Texture2D> defaultWhiteTexture;

		/*Circle*/
		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		/*Line*/
		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;

		/*Quad*/
		uint32_t quadIndicesToBeDrawnCount = 0;    //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		QuadVertex* quadVertexBufferData = nullptr;
		QuadVertex* quadVertexBufferDataPtr = nullptr;  //A pointer into the buffer to manipulate and control the buffer

		/*Circle*/
		uint32_t circleIndicesToBeDrawnCount = 0;    //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		CircleVertex* circleVertexBufferData = nullptr;
		CircleVertex* circleVertexBufferDataPtr = nullptr;  //A pointer into the buffer to manipulate and control the buffer

		/*Line*/
		uint32_t lineVerticesToBeDrawnCount = 0;    //Is incremented by 2 (2 vertices are required to draw a line) every time a line is added to the buffer
		LineVertex* lineVertexBufferData = nullptr;
		LineVertex* lineVertexBufferDataPtr = nullptr;  //A pointer into the buffer to manipulate and control the buffer
		float lineWidth = 2.0f;

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

		/*Quads*/
		s_data->quadVertexArray = VertexArray::Create();

		s_data->quadVertexBuffer = VertexBuffer::Create(s_data->MaxVerticesPerBatch * sizeof(QuadVertex));

		s_data->quadVertexBuffer->setLayout({
									  {VertexAttributeDataType::Float3, "Position"           },
									  {VertexAttributeDataType::Float4, "Color"				 },
									  {VertexAttributeDataType::Float2, "TextureCoordinates" },
									  {VertexAttributeDataType::Float,  "TextureIndex"		 },
									  {VertexAttributeDataType::Float,  "TextureSizeFactor"	 },
									  {VertexAttributeDataType::Int,    "EntityID"			 } 
			});

		s_data->quadVertexArray->addVertexBufferAndLinkLayout(s_data->quadVertexBuffer);

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

		/*
		* NOTE: quadIndexBuffer is also used for circles, as they have the same geometry (Circle is essentially a quad)
		*/
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data->MaxIndicesPerBatch);
		s_data->quadVertexArray->setIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;


		/*Circles*/
		s_data->circleVertexArray = VertexArray::Create();

		s_data->circleVertexBuffer = VertexBuffer::Create(s_data->MaxVerticesPerBatch * sizeof(CircleVertex));

		s_data->circleVertexBuffer->setLayout({
									  {VertexAttributeDataType::Float3, "WorldPosition" },
									  {VertexAttributeDataType::Float3, "LocalPosition" },
									  {VertexAttributeDataType::Float4, "Color"         },
									  {VertexAttributeDataType::Float,  "Thickness"     },
									  {VertexAttributeDataType::Float,  "Fade"	        },
									  {VertexAttributeDataType::Int,    "EntityID"      }
			});

		s_data->circleVertexArray->addVertexBufferAndLinkLayout(s_data->circleVertexBuffer);

		//Allocate enough storage for vertices
		s_data->circleVertexBufferData = new CircleVertex[s_data->MaxVerticesPerBatch];

		s_data->circleVertexArray->setIndexBuffer(quadIndexBuffer);  //Use quadIndexBuffer, as they have the same geometry (Circle is essentially a quad)


		/*Lines*/
		s_data->lineVertexArray = VertexArray::Create();

		s_data->lineVertexBuffer = VertexBuffer::Create(s_data->MaxVerticesPerBatch * sizeof(LineVertex));

		s_data->lineVertexBuffer->setLayout({
									  {VertexAttributeDataType::Float3, "Position" },
									  {VertexAttributeDataType::Float4, "Color"         },
									  {VertexAttributeDataType::Int,    "EntityID"      }
			});

		s_data->lineVertexArray->addVertexBufferAndLinkLayout(s_data->lineVertexBuffer);

		//Allocate enough storage for vertices
		/*
		 * NOTE: s_data->MaxVerticesPerBatch ===> allocates large number of vertices
		 * (should be refactored, lines are only used for visualizing physics, we don't need that many lines)
		 */
		s_data->lineVertexBufferData = new LineVertex[s_data->MaxVerticesPerBatch];


		s_data->defaultWhiteTexture = Texture2D::Create(1, 1);  //Create a one pixel texture
		uint32_t whiteTextureData = 0xffffffff;   //4-Bytes
		s_data->defaultWhiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		/*Load Shaders*/
		s_data->quadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		s_data->circleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
		s_data->lineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");

		/*Set The Texture Samplers Array*/
		int32_t textureSamplers[s_data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_data->MaxTextureSlots; i++) {
			textureSamplers[i] = i; 
		}
		s_data->quadShader->bind();  //Binding the shader before setting a uniform
		s_data->quadShader->setIntArray("u_textures", textureSamplers, s_data->MaxTextureSlots);


		s_data->textureSlots[0] = s_data->defaultWhiteTexture;    //Set the first element to our default white texture (no texture used and only the color is taken in the shader)

		//Set the positions for the initial quad in range [-0.5f to 0.5f] on XY axes, where (0.0f, 0.0f) is the origin with size of (1.0f, 1.0f) going clock-wise
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
		SetupShader(s_data->quadShader, camera.getProjectionMatrix() * glm::inverse(transform));
		SetupShader(s_data->circleShader, camera.getProjectionMatrix() * glm::inverse(transform));
		SetupShader(s_data->lineShader, camera.getProjectionMatrix() * glm::inverse(transform));

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewQuadsBatch();
		StartNewCirclesBatch();
		StartNewLinesBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& editorCamera)
	{
		SetupShader(s_data->quadShader, editorCamera.getViewProjection());
		SetupShader(s_data->circleShader, editorCamera.getViewProjection());
		SetupShader(s_data->lineShader, editorCamera.getViewProjection());

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewQuadsBatch();
		StartNewCirclesBatch();
		StartNewLinesBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SetupShader(s_data->quadShader, camera.getViewProjectionMatrix());
		SetupShader(s_data->circleShader, camera.getViewProjectionMatrix());
		SetupShader(s_data->lineShader, camera.getViewProjectionMatrix());

		/*Reset Scene Statistics*/
		ResetSceneStatistics();

		/*Reset Scene*/
		StartNewQuadsBatch();
		StartNewCirclesBatch();
		StartNewLinesBatch();
	}

	void Renderer2D::EndScene()
	{
		FlushQuads();
		FlushCircles();
		FlushLines();
	}
	/*
	* Sends the draw call for the batch
	*/
	void Renderer2D::FlushQuads()
	{
		/*Uploads The Data To The GPU, Does All The Neccessary Bindings And Issues A Draw Call*/

		//If there is no quads to render return from function
		if (s_data->quadIndicesToBeDrawnCount == 0)
			return;
		

		uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data->quadVertexBufferDataPtr) - reinterpret_cast<uint8_t*>(s_data->quadVertexBufferData));  //NOTE: Converted to uint8_t (1 Byte) to get the size and not the number of vertcies
		//Upload vertex buffer to GPU
		s_data->quadVertexArray->getVertexBuffers()[0]->setData(s_data->quadVertexBufferData, dataSize);
		

		/*Binding*/
		/*Bind All The Textures*/
		for (uint32_t i = 0; i < s_data->textureSlotIndex; i++) 
		{
			s_data->textureSlots[i]->bind(i);
		}
		s_data->quadVertexArray->bind();
		s_data->quadShader->bind();

		/*Draw Call*/
		RenderCommand::DrawIndices(s_data->quadVertexArray, s_data->quadIndicesToBeDrawnCount);

		/*STATISTICS*/
		//Keep track of draw calls count for scene statistics
		s_data->sceneStatistics.drawCallsCount++;
	}
	void Renderer2D::FlushCircles()
	{
		/*Uploads The Data To The GPU, Does All The Neccessary Bindings And Issues A Draw Call*/

		//If there is no circles to render return from function
		if (s_data->circleIndicesToBeDrawnCount == 0)
			return;


		uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data->circleVertexBufferDataPtr) - reinterpret_cast<uint8_t*>(s_data->circleVertexBufferData));  //NOTE: Converted to uint8_t (1 Byte) to get the size and not the number of vertcies
		//Upload vertex buffer to GPU
		s_data->circleVertexArray->getVertexBuffers()[0]->setData(s_data->circleVertexBufferData, dataSize);


		/*Binding*/
		s_data->circleVertexArray->bind();
		s_data->circleShader->bind();

		/*Draw Call*/
		RenderCommand::DrawIndices(s_data->circleVertexArray, s_data->circleIndicesToBeDrawnCount);

		/*STATISTICS*/
		//Keep track of draw calls count for scene statistics
		s_data->sceneStatistics.drawCallsCount++;
	}
	void Renderer2D::FlushLines()
	{
		/*Uploads The Data To The GPU, Does All The Neccessary Bindings And Issues A Draw Call*/

		//If there is no lines to render return from function
		if (s_data->lineVerticesToBeDrawnCount == 0)
			return;


		uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data->lineVertexBufferDataPtr) - reinterpret_cast<uint8_t*>(s_data->lineVertexBufferData));  //NOTE: Converted to uint8_t (1 Byte) to get the size and not the number of vertcies
		//Upload vertex buffer to GPU
		s_data->lineVertexArray->getVertexBuffers()[0]->setData(s_data->lineVertexBufferData, dataSize);


		/*Binding*/
		s_data->lineVertexArray->bind();
		s_data->lineShader->bind();

		/*Draw Call*/
		RenderCommand::DrawLines(s_data->lineVertexArray, s_data->lineVerticesToBeDrawnCount);

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

		//Check if the quads batch buffer is full
		if (IsQuadsBatchBufferFull()) {
			FlushQuads();
			StartNewQuadsBatch();
		}

		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec2 TextureCoordinates[4] = { {0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float TextureIndex = 0.0f; //Default white texture
		const float TextureSizeFactor = 1.0f; //Has no effect cuz we are not using a texture, only the default 1 pixel white texture, it is set to avoid warnings of unused uniform


		for (size_t i = 0; i < QuadVertexCount; i++) {
			AddQuadVertexToBuffer(transfromationMatrix * s_data->quadVertexPositions[i], color, TextureCoordinates[i], TextureIndex, TextureSizeFactor, entityID);
		}

		s_data->quadIndicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer

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

		//Check if the quads batch buffer is full
		if (IsQuadsBatchBufferFull()) {
			FlushQuads();
			StartNewQuadsBatch();
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
				FlushQuads();
				StartNewQuadsBatch();
			}
			textureIndex = float(s_data->textureSlotIndex);
			s_data->textureSlots[s_data->textureSlotIndex] = texture;
			s_data->textureSlotIndex++;

		}

		for (size_t i = 0; i < QuadVertexCount; i++) {
			AddQuadVertexToBuffer(transfromationMatrix * s_data->quadVertexPositions[i], tintColor, TextureCoordinates[i], textureIndex, textureSizeFactor, entityID);
		}

		s_data->quadIndicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer
		
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
		//With texture
		if (spriteRendererComponent.texture) //if texture is not nullptr
		{
			DrawQuad(transformationMatrix, spriteRendererComponent.texture, spriteRendererComponent.textureSizeFactor, spriteRendererComponent.color, entityID);
		}
		//Without texture
		else
		{
			//Renders with the default (1 * 1) white texture, has the effect of not using a texture
			DrawQuad(transformationMatrix, spriteRendererComponent.color, entityID);
		}
	}

	void Renderer2D::DrawCircle(const glm::mat4& transfromationMatrix, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		/*
		* NOTE: Drawing a circle is essentially drawing a quad with a different shader, and it has a different vertex layout.
		* Quad vertices goes counter clock-wise.
		*/

		//Check if the circles batch buffer is full
		if (IsCirclesBatchBufferFull()) {
			FlushCircles();
			StartNewCirclesBatch();
		}

		constexpr size_t CircleVertexCount = 4;

		for (size_t i = 0; i < CircleVertexCount; i++) {
			AddCircleVertexToBuffer(transfromationMatrix * s_data->quadVertexPositions[i]
									, s_data->quadVertexPositions[i] * 2.0f    //Sets the range [-1.0f to 1.0f] on XY axes, as that the range of s_data->quadVertexPositions is [-0.5f to 0.5f]
									, color, thickness, fade, entityID);
		}

		s_data->circleIndicesToBeDrawnCount += 6;   //Is incremented by 6 (6 indices are required to draw a quad) every time a quad is added to the buffer

		/*STATISTICS*/
		//Keep track of quad count for scene statistics
		s_data->sceneStatistics.quadCount++;   //NOTE: Increase the quadCount and not creating a new stats for circleCount, as that drawing a circle is ultimately drawing a quad
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		//Check if the lines batch buffer is full
		if (IsLinesBatchBufferFull()) {
			FlushLines();
			StartNewLinesBatch();
		}

		AddLineVertexToBuffer(p0, color, entityID);
		AddLineVertexToBuffer(p1, color, entityID);

		s_data->lineVerticesToBeDrawnCount += 2;   //Is incremented by 2 (2 vertices are required to draw a line) every time a line is added to the buffer

		/*STATISTICS*/
		//Keep track of line count for scene statistics
		//s_data->sceneStatistics.lineCount++;    ===> TODO, not implemented yet
	}

	void Renderer2D::DrawRectangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		//Going clock-wise
		glm::vec3 p0 = glm::vec3(position.x - (size.x * 0.5f), position.y - (size.y * 0.5f), position.z);  //Bottom-Left corner
		glm::vec3 p1 = glm::vec3(position.x + (size.x * 0.5f), position.y - (size.y * 0.5f), position.z);  //Bottom-Right corner
		glm::vec3 p2 = glm::vec3(position.x + (size.x * 0.5f), position.y + (size.y * 0.5f), position.z);  //Top-Right corner
		glm::vec3 p3 = glm::vec3(position.x - (size.x * 0.5f), position.y + (size.y * 0.5f), position.z);  //Top-Left corner

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRectangle(const glm::mat4& transfromationMatrix, const glm::vec4& color, int entityID)
	{	
		glm::vec3 rectangleVertices[4];
		for (size_t i = 0; i < 4; i++) {
			rectangleVertices[i] = transfromationMatrix * s_data->quadVertexPositions[i]; //Using s_data->quadVertexPositions, as it is centered at the origin (0.0f, 0.0f) with size of (1.0f, 1.0f)
		}

		DrawLine(rectangleVertices[0], rectangleVertices[1], color, entityID);
		DrawLine(rectangleVertices[1], rectangleVertices[2], color, entityID);
		DrawLine(rectangleVertices[2], rectangleVertices[3], color, entityID);
		DrawLine(rectangleVertices[3], rectangleVertices[0], color, entityID);
	}

	float Renderer2D::getLineWidth()
	{
		return s_data->lineWidth;
	}

	void Renderer2D::setLineWidth(float width)
	{
		s_data->lineWidth = width;
		RenderCommand::SetLineWidth(s_data->lineWidth); //NOTE: It calls an "OpenGL global" that just changes the state, which just need to be done once whenever the width is changed
	}

	void Renderer2D::StartNewQuadsBatch()
	{
		/*Reset The Quad Variables*/
		s_data->quadIndicesToBeDrawnCount = 0;
		s_data->quadVertexBufferDataPtr = s_data->quadVertexBufferData;  //Let the pointer point to the begining of the buffer

		s_data->textureSlotIndex = 1;   //Resets to 1, cause index 0 is occupied by the default white texture
	}

	void Renderer2D::StartNewCirclesBatch()
	{
		/*Reset The Circle Variables*/
		s_data->circleIndicesToBeDrawnCount = 0;
		s_data->circleVertexBufferDataPtr = s_data->circleVertexBufferData;  //Let the pointer point to the begining of the buffer
	}

	void Renderer2D::StartNewLinesBatch()
	{
		/*Reset The Line Variables*/
		s_data->lineVerticesToBeDrawnCount = 0;
		s_data->lineVertexBufferDataPtr = s_data->lineVertexBufferData;  //Let the pointer point to the begining of the buffer
	}

	void Renderer2D::SetupShader(const Ref<Shader> shader, const glm::mat4& viewProjectionMatrix)
	{
		shader->bind();
		shader->setMat4("u_viewProjection", viewProjectionMatrix); //Set the uniform
	}

	bool Renderer2D::IsQuadsBatchBufferFull()
	{
		/*
		* Returns true if the quads buffer is full
		*/
		if (s_data->quadIndicesToBeDrawnCount >= Renderer2DData::MaxIndicesPerBatch) {
			return true;
		}
		return false;
	}

	bool Renderer2D::IsCirclesBatchBufferFull()
	{
		/*
		* Returns true if the circles buffer is full
		*/
		if (s_data->circleIndicesToBeDrawnCount >= Renderer2DData::MaxIndicesPerBatch) {
			return true;
		}
		return false;
	}

	bool Renderer2D::IsLinesBatchBufferFull()
	{
		/*
		* Returns true if the lines buffer is full
		*/
		if (s_data->lineVerticesToBeDrawnCount >= Renderer2DData::MaxVerticesPerBatch) {
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

	void Renderer2D::AddQuadVertexToBuffer(const glm::vec3& position, const glm::vec4& color,const glm::vec2& textureCoordinates, float textureIndex, float textureSizeFactor, int entityID)
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

	void Renderer2D::AddCircleVertexToBuffer(const glm::vec3& worldPosition, const glm::vec3& localPosition, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		//circleVertexBufferDataPtr points to the first empty space in the buffer
		s_data->circleVertexBufferDataPtr->worldPosition = worldPosition;
		s_data->circleVertexBufferDataPtr->localPosition = localPosition;
		s_data->circleVertexBufferDataPtr->color = color;
		s_data->circleVertexBufferDataPtr->Thickness = thickness;
		s_data->circleVertexBufferDataPtr->Fade = fade;
		s_data->circleVertexBufferDataPtr->entityID = entityID;
		s_data->circleVertexBufferDataPtr++; //Increment pointer to the next vertex
	}

	void Renderer2D::AddLineVertexToBuffer(const glm::vec3& position, const glm::vec4& color, int entityID)
	{
		//lineVertexBufferDataPtr points to the first empty space in the buffer
		s_data->lineVertexBufferDataPtr->position = position;
		s_data->lineVertexBufferDataPtr->color = color;
		s_data->lineVertexBufferDataPtr->entityID = entityID;
		s_data->lineVertexBufferDataPtr++; //Increment pointer to the next vertex
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