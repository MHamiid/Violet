#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneCamera.h"
#include "Script.h"
#define GLM_ENABLE_EXPERIMENTAL
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
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };  //In radians
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) {}

		TransformComponent(const TransformComponent&) = default;

		glm::mat4 getTransform() const {
			/*
			 * NOTE: The rotation matrix order must match with ImGuizmos rotation order in Violet-Editor (ImGuizmos takes a mat4 and writes to it the new transformation for the object)
			 */
			glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotation));
			
			return glm::translate(glm::mat4(1.0f), translation)
				* rotationMatrix
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct VIOLET_API SpriteRendererComponent
	{
		/*TODO: Should Have A Material When Material System Is Added*/
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}

		SpriteRendererComponent(const SpriteRendererComponent&) = default;

	};

	struct VIOLET_API CameraComponent
	{
		SceneCamera sceneCamera;
		bool fixedAspectRatio = false;

		CameraComponent() = default;

		CameraComponent(const CameraComponent&) = default;

	};

	struct VIOLET_API NativeScriptComponent
	{
		Script* script = nullptr;

		Script* (*instantiateScriptFUNC)();
		void (*destroyScriptFUNC)(NativeScriptComponent*);

		NativeScriptComponent() = default;

		NativeScriptComponent(const NativeScriptComponent&) = default;

		template<typename T>
		void bind() {
			instantiateScriptFUNC = []() {return static_cast<Script*>(new T()); };
			destroyScriptFUNC = [](NativeScriptComponent* thisNativeScriptComponent) { delete thisNativeScriptComponent->script; thisNativeScriptComponent->script = nullptr; };
		
		}

	};
}