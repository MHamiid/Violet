#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Violet {

	class VIOLET_API Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//#########//
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		/*
		* Default color is white means use the texture with no tented colors
		*/
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	};
}
