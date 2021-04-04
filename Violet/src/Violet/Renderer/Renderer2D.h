#pragma once
#include "Violet/Renderer/OrthographicCamera.h"
#include "Violet/Renderer/Texture.h"
#include "Violet/Renderer/Camera.h"

namespace Violet {

	class VIOLET_API Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);  //TODO: Remove
		static void EndScene();
		static void Flush();

		//#########//
		/*
			QUADS
		*/
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::mat4& transfromationMatrix, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		/*
		* Default color is white means use the texture with no tented colors
		*/
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureSizeFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::mat4& transfromationMatrix, const Ref<Texture2D>& texture, float textureSizeFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		/*
		* 
		* ROTATED QUADS
		* NOTE: Created a Rotated function for rotated quad to avoid the overhead of creating a rotation matrix and multiplication when it is not needed.
		* 
		*/
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		/*
		* Default color is white means use the texture with no tented colors
		*/
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float textureSizeFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, float textureSizeFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		/*
		* Statistics
		*/
		struct SceneStatistics {
			uint32_t drawCallsCount = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalDrawCallsCount() const { return drawCallsCount; }
			uint32_t getTotalQuadCount() const { return quadCount; }
			uint32_t getTotalVertexCount() const { return quadCount * 4; }  //Each quad contains 4 vertices
			uint32_t getTotalIndexCount() const { return quadCount * 6; }   //Each quad contains 6 indices
		};

		static SceneStatistics GetSceneStatistics();  //Return statistics for the last scene after calling BeginScene()
		static void ResetSceneStatistics();

	private:
		static void StartNewBatch();
		static bool IsBatchBufferFull();
		static bool IsTextureSlotsFull();
		static void AddVertexToBuffer(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoordinates, float textureIndex, float textureSizeFactor);

	};
}
