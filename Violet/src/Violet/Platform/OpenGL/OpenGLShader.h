#pragma once
#include "Violet/Renderer/Shader.h"

namespace Violet {
	
	class VIOLET_API OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();
		virtual void bind() override;
		virtual void unBind() override;
	private:
		uint32_t m_programID;
	};


}