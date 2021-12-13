#include "VIOPCH.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Entity.h"
#include "Components.h"
#include "Violet/Core/UUID.h"

/*Handling YAML::Node.as<glm::vec2>(), YAML::Node.as<glm::vec3>(), YAML::Node.as<glm::vec4>(), YAML::Node.as<UUID>()*/
namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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

	template<>
	struct convert<Violet::UUID>
	{
		static Node encode(const Violet::UUID& rhs)
		{
			Node node;
			node.push_back(rhs);
			return node;
		}

		static bool decode(const Node& node, Violet::UUID& rhs)
		{
			rhs = node.as<uint64_t>();
			return true;
		}
	};

}

namespace Violet {
	/*Helper Functions*/
	static std::string RidgidBody2DBodyTypeToString(RidgidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RidgidBody2DComponent::BodyType::Static:     return "Static";
			case RidgidBody2DComponent::BodyType::Dynamic:	  return "Dynamic";
			case RidgidBody2DComponent::BodyType::Kinematic:  return "Kinematic";
		}

		VIO_CORE_ASSERT(false, "[Scene Serializer] Unknown Body Type");
		return std::string();
	}

	static RidgidBody2DComponent::BodyType RidgidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")			 return RidgidBody2DComponent::BodyType::Static;
		else if (bodyTypeString == "Dynamic")	 return RidgidBody2DComponent::BodyType::Dynamic;
		else if (bodyTypeString == "Kinematic")  return RidgidBody2DComponent::BodyType::Kinematic;

		VIO_CORE_ASSERT(false, "[Scene Serializer] Unknown Body Type");
		return RidgidBody2DComponent::BodyType::Static;
	}
}

namespace Violet {

	/*Create Yaml Shift Operator Overloads To Take glm::vec2, glm::vec3, glm::vec4*/

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) 
	{
		out << YAML::Flow;  //Set the output to be in one line [x, y, z, .....]
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
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
		VIO_CORE_ASSERT(entity.hasComponent<IDComponent>(), "Entity Does Not Have IDComponent!");

		out << YAML::BeginMap;  //Entity

		out << YAML::Key << "EntityID" << YAML::Value << entity.getUUID();

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

		/*CircleRenderer Component*/
		if (entity.hasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";

			out << YAML::BeginMap; //CircleRenderer Component

			CircleRendererComponent& circleRendererComponent = entity.getComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap;   //CircleRenderer Component
		}

		/*RidgidBody2D Component*/
		if (entity.hasComponent<RidgidBody2DComponent>())
		{
			out << YAML::Key << "RidgidBody2DComponent";

			out << YAML::BeginMap; //RidgidBody2D Component

			RidgidBody2DComponent& rb2dComponent = entity.getComponent<RidgidBody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RidgidBody2DBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

			out << YAML::EndMap;   //RidgidBody2D Component
		}

		/*BoxCollider2D Component*/
		if (entity.hasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";

			out << YAML::BeginMap; //BoxCollider2D Component

			BoxCollider2DComponent& bc2dComponent = entity.getComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			out << YAML::Key << "SizeFactor" << YAML::Value << bc2dComponent.SizeFactor;
			out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;   //BoxCollider2D Component
		}

		/*CircleCollider2D Component*/
		if (entity.hasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";

			out << YAML::BeginMap; //CircleCollider2D Component

			CircleCollider2DComponent& cc2dComponent = entity.getComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;   //CircleCollider2D Component
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
			out << YAML::Key << "PrimaryCameraEntityID" << YAML::Value << m_scene->getPrimaryCameraEntity().getUUID();
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
		/*Fix Program Crashing When The File Is Invalid (not a scene file, a random file)*/
		YAML::Node sceneData = YAML::LoadFile(filePath);
		if (!sceneData["Scene"]) //The file data doesn't contain the Scene node 
		{
			return false;
		}

		//Set the scene name
		m_scene->m_sceneName = sceneData["Scene"].as<std::string>().c_str();

		YAML::Node primaryCameraEntityIDNode = sceneData["PrimaryCameraEntityID"];
		UUID primaryCameraEntityID = -1;  //Initialize with a non-valid entity ID (?? Is it really non-valid entity ID?)

		if (primaryCameraEntityIDNode) 
		{
			primaryCameraEntityID = primaryCameraEntityIDNode.as<UUID>();
		}


		YAML::Node entities = sceneData["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				/*ID Component*/
				UUID uuid = entity["EntityID"].as<UUID>();

				/*Tag Component*/
				YAML::Node tagComponentNode = entity["TagComponent"];

				std::string tag;
				if (tagComponentNode)
				{
					tag = tagComponentNode["Tag"].as<std::string>();
				}
				Entity deserializedEntity = m_scene->createEntityWithUUID(uuid ,tag);


				YAML::Node entityIDNode = entity["EntityID"];

				/*Check is this entity is the primaryCameraEntity*/
				if (entityIDNode && primaryCameraEntityID != -1)
				{
					UUID entityID = entityIDNode.as<UUID>();
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

				/*CircleRenderer Component*/
				YAML::Node circleComponentNode = entity["CircleRendererComponent"];

				if (circleComponentNode)
				{
					CircleRendererComponent& circleRendererComponent = deserializedEntity.addComponent<CircleRendererComponent>();
					circleRendererComponent.color = circleComponentNode["Color"].as<glm::vec4>();
					circleRendererComponent.Thickness = circleComponentNode["Thickness"].as<float>();
					circleRendererComponent.Fade = circleComponentNode["Fade"].as<float>();
				}

				/*RidgidBody2D Component*/
				YAML::Node rb2dComponentNode = entity["RidgidBody2DComponent"];

				if (rb2dComponentNode)
				{
					RidgidBody2DComponent& rb2dComponent = deserializedEntity.addComponent<RidgidBody2DComponent>();
					rb2dComponent.Type = RidgidBody2DBodyTypeFromString(rb2dComponentNode["BodyType"].as<std::string>());
					rb2dComponent.FixedRotation = rb2dComponentNode["FixedRotation"].as<bool>();
				}

				/*BoxCollider2D Component*/
				YAML::Node bc2dComponentNode = entity["BoxCollider2DComponent"];

				if (bc2dComponentNode)
				{
					BoxCollider2DComponent& bc2dComponent = deserializedEntity.addComponent<BoxCollider2DComponent>();
					bc2dComponent.Offset = bc2dComponentNode["Offset"].as<glm::vec2>();
					bc2dComponent.SizeFactor = bc2dComponentNode["SizeFactor"].as<glm::vec2>();
					bc2dComponent.Density = bc2dComponentNode["Density"].as<float>();
					bc2dComponent.Friction = bc2dComponentNode["Friction"].as<float>();
					bc2dComponent.Restitution = bc2dComponentNode["Restitution"].as<float>();
					bc2dComponent.RestitutionThreshold = bc2dComponentNode["RestitutionThreshold"].as<float>();
				}

				/*CircleCollider2D Component*/
				YAML::Node cc2dComponentNode = entity["CircleCollider2DComponent"];

				if (cc2dComponentNode)
				{
					CircleCollider2DComponent& cc2dComponent = deserializedEntity.addComponent<CircleCollider2DComponent>();
					cc2dComponent.Offset = cc2dComponentNode["Offset"].as<glm::vec2>();
					cc2dComponent.Radius = cc2dComponentNode["Radius"].as<float>();
					cc2dComponent.Density = cc2dComponentNode["Density"].as<float>();
					cc2dComponent.Friction = cc2dComponentNode["Friction"].as<float>();
					cc2dComponent.Restitution = cc2dComponentNode["Restitution"].as<float>();
					cc2dComponent.RestitutionThreshold = cc2dComponentNode["RestitutionThreshold"].as<float>();
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