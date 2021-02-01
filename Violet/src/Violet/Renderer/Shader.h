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
	public:
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};

}