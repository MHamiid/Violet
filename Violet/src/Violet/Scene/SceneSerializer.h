#pragma once
#include "Scene.h"

namespace Violet {

	class VIOLET_API SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void serializeToText(const std::string& filePath);
		void serializeToBinary(const std::string& filePath);

		bool deserializeText(const std::string& filePath);
		bool deserializeBinary(const std::string& filePath);
	private:
		Ref<Scene> m_scene;   //The scene context for Serialization/Deserialization
	
	
	};

}