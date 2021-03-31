#pragma once
#include <glm/glm.hpp>

namespace Violet {

	struct VIOLET_API TransformComponent
	{
		glm::mat4 transformation{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transfrom) : transformation(transfrom) {}

		TransformComponent(const TransformComponent&) = default;

		//For casting
		operator glm::mat4& () { return transformation; }
		operator const glm::mat4& () const { return transformation; }
	};

	struct VIOLET_API SpriteRendererComponent
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}

		SpriteRendererComponent(const SpriteRendererComponent&) = default;

	};

}