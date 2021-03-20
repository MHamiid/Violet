#pragma once
#include "OrthographicCamera.h"

namespace Violet {

	class VIOLET_API Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//#########//
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationZ = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationZ = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	};
}
