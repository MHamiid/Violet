#pragma once
#include "Violet/Renderer/Shader.h"
#include <glad/glad.h>

namespace Violet {
	
	class VIOLET_API OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void bind() override;
		virtual void unBind() override;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) override; //Temp
		virtual void setFloat4(const std::string& name, const glm::vec4& values) override; //Temp
		virtual void setInt(const std::string& name, int value) override;//Temp
	private:
		std::unordered_map<GLenum, std::string> parseShadersFile(const std::string& filePath);
		void createProgram(const std::unordered_map<GLenum, std::string>& shaderSources);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
		virtual void uploadUniformInt(const std::string& name, int value);
		
	private:
		uint32_t m_programID;
	};


}