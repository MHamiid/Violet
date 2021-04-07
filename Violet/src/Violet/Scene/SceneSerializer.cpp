#include "VIOPCH.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Entity.h"
#include "Components.h"

/*Handling YAML::Node.as<glm::vec3>() and YAML::Node.as<glm::vec4>()*/
namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Violet {

	/*Create Yaml Shift Operator Overloads To Take glm::vec3, glm::vec4*/

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) 
	{
		out << YAML::Flow;  //Set the output to be in one line [x, y, z, .....]
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;  //Set the output to be in one line [x, y, z, .....]
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;  //Entity

		out << YAML::Key << "EntityID" << YAML::Value << (uint32_t)(entt::entity)entity;

		/*Tag Component*/
		if (entity.hasComponent<TagComponent>()) 
		{
			out << YAML::Key << "TagComponent";

			out << YAML::BeginMap; //Tag Component

			auto& tag = entity.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag; 

			out << YAML::EndMap;   //Tag Component
		}

		/*Transform Component*/
		if (entity.hasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";

			out << YAML::BeginMap; //Transform Component

			TransformComponent& transformComponent = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComponent.translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;

			out << YAML::EndMap;   //Transform Component
		}

		/*Camera Component*/
		if (entity.hasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";

			out << YAML::BeginMap; //Camera Component

			CameraComponent& cameraComponent = entity.getComponent<CameraComponent>();
			SceneCamera& sceneCamera = cameraComponent.sceneCamera;

			out << YAML::Key << "SceneCamera" << YAML::Value;
			out << YAML::BeginMap; //Scene Camera

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)sceneCamera.getProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << sceneCamera.getPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << sceneCamera.getPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << sceneCamera.getPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << sceneCamera.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << sceneCamera.getOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << sceneCamera.getOrthographicFarClip();

			out << YAML::EndMap;   //Scene Camera


			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap;   //Camera Component
		}


		/*SpriteRenderer Component*/
		if (entity.hasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";

			out << YAML::BeginMap; //SpriteRenderer Component

			SpriteRendererComponent& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

			out << YAML::EndMap;   //SpriteRenderer Component
		}

		out << YAML::EndMap;    //Entity
	}

	void SceneSerializer::serializeToText(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;  //Scene

		out << YAML::Key << "Scene" << YAML::Value << m_scene->getSceneName().c_str();
		if (m_scene->getPrimaryCameraEntity())  //If there is a primary camera entity in the scene
		{
			out << YAML::Key << "PrimaryCameraEntityID" << YAML::Value << (uint32_t)(entt::entity)m_scene->getPrimaryCameraEntity();
		}
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;  //Array of values

		/*Iterate Over All Entities*/
		m_scene->m_registry.each([&](entt::entity enttID) 
		{
				Entity entity = { enttID, m_scene.get() };
				if (!entity) //If the entity is not valid
				{
					return;
				}

				SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;  //End the array of values for the entities
		out << YAML::EndMap;  //Scene

		//TODO: Create the directory in the filePath if not exists
		std::ofstream fileOut(filePath);  //Assumes that the directory in the filePath exists
		fileOut << out.c_str();

		VIO_CORE_TRACE("Serialized Scene {0} ===> {1}", m_scene->getSceneName().c_str(), filePath.c_str());
	}

	void SceneSerializer::serializeToBinary(const std::string& filePath)
	{
		VIO_CORE_ASSERT(false, "Not Implemented Yet!");
	}

	bool SceneSerializer::deserializeText(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream stringStream;
		stringStream << stream.rdbuf();

		YAML::Node sceneData = YAML::Load(stringStream.str());
		if (!sceneData["Scene"]) //The file data doesn't contain the Scene node 
		{
			return false;
		}

		//Set the scene name
		m_scene->m_sceneName = sceneData["Scene"].as<std::string>().c_str();

		YAML::Node primaryCameraEntityIDNode = sceneData["PrimaryCameraEntityID"];
		uint32_t primaryCameraEntityID;

		if (primaryCameraEntityIDNode) 
		{
			primaryCameraEntityID = primaryCameraEntityIDNode.as<uint32_t>();
		}


		YAML::Node entities = sceneData["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				/*Tag Component*/
				YAML::Node tagComponentNode = entity["TagComponent"];

				std::string tag;
				if (tagComponentNode)
				{
					tag = tagComponentNode["Tag"].as<std::string>();
				}
				Entity deserializedEntity = m_scene->createEntity(tag);


				/*Entity(entt::entity) ID*/
				YAML::Node entityIDNode = entity["EntityID"];

				if (entityIDNode)
				{
					uint32_t entityID = entityIDNode.as<uint32_t>();
					if (entityID == primaryCameraEntityID) //If this entity is the primary camera entity
					{
						m_scene->setPrimaryCameraEntity(deserializedEntity);
					}
				}


				/*Transform Component*/
				YAML::Node transformComponentNode = entity["TransformComponent"];
				if (transformComponentNode)
				{
					//NOTE: Entities are created with tag component and transform component attached to it by default in the Scene.createEntity() function
					TransformComponent& transformComponent = deserializedEntity.getComponent<TransformComponent>();  //Get the attached transform component
					transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
					transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
					transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
				}


				/*Camera Component*/
				YAML::Node cameraComponentNode = entity["CameraComponent"];
				if (cameraComponentNode)
				{
					CameraComponent& cameraComponent = deserializedEntity.addComponent<CameraComponent>();
					YAML::Node sceneCameraNode = cameraComponentNode["SceneCamera"];

					cameraComponent.sceneCamera.setProjectionType((SceneCamera::ProjectionType)sceneCameraNode["ProjectionType"].as<int>());

					cameraComponent.sceneCamera.setPerspectiveVerticalFOV(sceneCameraNode["PerspectiveFOV"].as<float>());
					cameraComponent.sceneCamera.setPerspectiveNearClip(sceneCameraNode["PerspectiveNear"].as<float>());
					cameraComponent.sceneCamera.setPerspectiveFarClip(sceneCameraNode["PerspectiveFar"].as<float>());

					cameraComponent.sceneCamera.setOrthographicSize(sceneCameraNode["OrthographicSize"].as<float>());
					cameraComponent.sceneCamera.setOrthographicNearClip(sceneCameraNode["OrthographicNear"].as<float>());
					cameraComponent.sceneCamera.setOrthographicFarClip(sceneCameraNode["OrthographicFar"].as<float>());

					cameraComponent.fixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();

				}


				/*SpriteRenderer Component*/
				YAML::Node spriteComponentNode = entity["SpriteRendererComponent"];

				if (spriteComponentNode)
				{
					SpriteRendererComponent& spritRendererComponent = deserializedEntity.addComponent<SpriteRendererComponent>();
					spritRendererComponent.color = spriteComponentNode["Color"].as<glm::vec4>();
				}

			}
		}

		VIO_CORE_TRACE("Deserialized Scene {0} ===> {1}", filePath.c_str(), sceneData["Scene"].as<std::string>().c_str());
		return true;
	}

	bool SceneSerializer::deserializeBinary(const std::string& filePath)
	{
		VIO_CORE_ASSERT(false, "Not Implemented Yet!");
		return false;
	}

}