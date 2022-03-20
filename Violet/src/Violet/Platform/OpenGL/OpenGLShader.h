#pragma once
#include "Violet/Renderer/Shader.h"
#include <glad/glad.h>

namespace Violet {
	
	class VIOLET_API OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& shaderName, const std::string& filePath);
		OpenGLShader(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void bind() override;
		virtual void unBind() override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void setFloat(const std::string& name, float value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual const std::string& getName() const override { return m_shaderName; }
	private:
		std::unordered_map<GLenum, std::string> parseShadersFile(const std::string& filePath);
		void createProgram(const std::unordered_map<GLenum, std::string>& shaderSources);
		void uploadUniformInt(const std::string& name, int value);
		void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string shaderTypeToString(GLenum shaderType);
	private:
		uint32_t m_programID;
		std::string m_shaderName;
	};


}