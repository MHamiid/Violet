#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
namespace Violet {


	struct VIOLET_API TagComponent
	{
		std::string tag = "Undefined Tag";

		TagComponent() = default;
		TagComponent(const std::string& tag) : tag(tag) {}

		TagComponent(const TagComponent&) = default;
	};

	struct VIOLET_API TransformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) {}

		TransformComponent(const TransformComponent&) = default;

		glm::mat4 getTransform() const {
			/*
			 * TODO: Check if this rotationMatrix works correctly for 3D
			 * TODO: Find more optimized matrix multiplication for the rotationMatrix for performance
			 */
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f });
			rotationMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f });
			rotationMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f });
			
			return glm::translate(glm::mat4(1.0f), translation)
				* rotationMatrix
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct VIOLET_API SpriteRendererComponent
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}

		SpriteRendererComponent(const SpriteRendererComponent&) = default;

	};

}