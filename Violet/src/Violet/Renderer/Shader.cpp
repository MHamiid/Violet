#include "VIOPCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Violet/Platform/OpenGL/OpenGLShader.h"
namespace Violet {

	/*
		 //////////////////////////////////////////////////////
		//////////////////Shader Implementation///////////////
	   //////////////////////////////////////////////////////
	*/
	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			//VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLShader>(filePath);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& shaderName, const std::string& filePath)
	{
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			//VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLShader>(shaderName, filePath);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
		switch (Renderer::GetCurrentGraphicsAPI())
		{
		case GraphicsAPI::API::NONE:
			VIO_CORE_ASSERT(false, "[Renderer] No Renderer API Specified!");
			return nullptr;

		case GraphicsAPI::API::OPENGL:
			//VIO_CORE_DEBUG("[Renderer] OpenGL Selected");
			return CreateRef<OpenGLShader>(shaderName, vertexSrc, fragmentSrc);
		}

		VIO_CORE_ASSERT(false, "[Renderer] Renderer API Selection Failed!");
		return nullptr;
    }

	/*
		 /////////////////////////////////////////////////////////////
		//////////////////Shader Library Implementation//////////////
	   /////////////////////////////////////////////////////////////
	*/

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{	
		const std::string& shaderName = shader->getName();
		if (m_shaders.find(shaderName) == m_shaders.end()) {
			m_shaders[shaderName] = shader;
			VIO_CORE_DEBUG("[Shader Library] Added '{0}'", shaderName);
		}
		else { VIO_CORE_WARN("[Shader Library] Addition Ignored, Shader '{0}' Already Exists!", shaderName); }

	}
	Ref<Shader> ShaderLibrary::createAndAdd(const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::createAndAdd(const std::string& shaderName, const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(shaderName ,filePath);
		add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::get(const std::string& shaderName)
	{	
		VIO_CORE_ASSERT(shaderExists(shaderName), "[Shader Library] Shader Not Found!");
		return m_shaders[shaderName];
	}
	bool ShaderLibrary::shaderExists(const std::string& shaderName)
	{
		return m_shaders.find(shaderName) != m_shaders.end();
	}
}
