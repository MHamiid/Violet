#pragma once
#include <glm/glm.hpp>

namespace Violet {

	/*
	Interface
	*/
	class VIOLET_API Shader {
	public:
		virtual ~Shader() = default;
		virtual void bind() = 0;
		virtual void unBind() = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) = 0; //Temp
		virtual void setFloat4(const std::string& name, const glm::vec4& values) = 0; //Temp
		virtual void setInt(const std::string& name, int value) = 0; //Temp
		virtual const std::string& getName() const = 0;
	public:
		static Ref<Shader> Create(const std::string& filePath);  //shaderName will be named after the file name in the filePath
		static Ref<Shader> Create(const std::string& shaderName, const std::string& filePath);
		static Ref<Shader> Create(const std::string& shaderName,const std::string& vertexSrc, const std::string& fragmentSrc);

	};

	class VIOLET_API ShaderLibrary {
	public:
		void add(const Ref<Shader>& shader);
		Ref<Shader> createAndAdd(const std::string& filePath);  //shaderName will be named after the file name in the filePath
		Ref<Shader> createAndAdd(const std::string& shaderName, const std::string& filePath);

		Ref<Shader> get(const std::string& shaderName);
		bool shaderExists(const std::string& shaderName);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;  //std::string is a unique name that identify the shader
	};

}