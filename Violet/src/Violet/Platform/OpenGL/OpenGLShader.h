#pragma once
#include "Violet/Renderer/Shader.h"

namespace Violet {
	
	class VIOLET_API OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void bind() override;
		virtual void unBind() override;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) override;
	private:
		uint32_t m_programID;
	};


}