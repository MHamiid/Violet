#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneCamera.h"
#include "Script.h"
#include "Violet/Renderer/Texture.h"
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
		Ref<Texture2D> texture;
		float textureSizeFactor = 1.0;

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

	/*Physics*/
	struct VIOLET_API RidgidBody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;  //Set a default BodyType
		bool FixedRotation = false;

		//Storage for b2Body that is created using this component data
		void* RuntimeBody = nullptr;

		RidgidBody2DComponent() = default;
		RidgidBody2DComponent(const RidgidBody2DComponent&) = default;
	};

	struct VIOLET_API BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };		  //To set the collider to be offset from the middle of the entity (from the transfrom position)
		glm::vec2 SizeFactor = { 0.5f, 0.5f };    //Size Factor from the middle of the entity (from the transfrom position)

		//TODO: Move these into physics material??
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;   //Bounciness
		float RestitutionThreshold = 0.5f;   //Restitution velocity threshold, usually in m / s.Collisions above this speed have restitution applied(will bounce)

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}