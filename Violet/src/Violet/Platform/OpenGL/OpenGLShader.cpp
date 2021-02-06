#include "VIOPCH.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
namespace Violet {
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_programID(0) //initialize with 0
	{	
		//Parse the file content
		std::unordered_map<GLenum, std::string> shaderSources= parseShadersFile(filePath);
		createProgram(shaderSources);

	}
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) : m_programID(0) //initialize with 0
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		createProgram(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{	
		glDeleteProgram(m_programID);
	}

	void OpenGLShader::bind()
	{
		glUseProgram(m_programID);
	}

	void OpenGLShader::unBind()
	{
		glUseProgram(0);
	}

	//Temp
	//Assumes that shader is bound first
	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix)
	{
		uploadUniformMat4(name, matrix);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& values)
	{
		uploadUniformFloat4(name, values);
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		uploadUniformInt(name, value);
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::parseShadersFile(const std::string& filePath)
	{
		std::unordered_map<GLenum, std::string> shaderSourceCodes;
		std::string buffer;

		enum class ShaderType : GLenum {
			NONE = 0, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER
		};
		ShaderType type = ShaderType::NONE;


		//Read the content of the file
		std::ifstream stream(filePath);
		VIO_CORE_ASSERT(stream, "[OpenGL]Couldn't Open Shaders File!");
		if (stream) {
			std::string line;
			while (getline(stream, line)) {
				//Split the shader source code into Vertex Shader and Fragment Shader
				if (line.find("#type") != std::string::npos) {
					if (!buffer.empty()) {
						shaderSourceCodes[(GLenum)type] = buffer;
						buffer.clear();
					}
					if (line.find("vertex") != std::string::npos) {	
						type = ShaderType::VERTEX;
					}
					else if (line.find("fragment") != std::string::npos) {
						type = ShaderType::FRAGMENT;
					}
				}
				else {
					buffer.append(line + "\n");
				}
			}

			VIO_CORE_ASSERT(!(type == ShaderType::NONE) , "[OpenGL]Couldn't Read Shaders!");
			shaderSourceCodes[(GLenum)type] = buffer;
		}
		
		
		return shaderSourceCodes;
	}

	void OpenGLShader::createProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Get a program object.
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
		for (auto&& [shaderType, shaderSourceCode] : shaderSources) {
		
			GLuint shader = glCreateShader(shaderType);
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)shaderSourceCode.c_str();
			glShaderSource(shader, 1, &source, 0);
			// Compile the shader
			glCompileShader(shader);
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				VIO_CORE_ERROR("[OpenGL] shader compilation error!");
				VIO_CORE_ERROR("{0}", infoLog.data());
				//Break the program
				VIO_CORE_ASSERT(false, "");
				return; //Exit the function
			}
			// shader is successfully compiled.
			// Now time to link them together into a program.
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		 }

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto shaderID : glShaderIDs) {
				glDeleteShader(shaderID);
			}

			// Use the infoLog as you see fit.
			VIO_CORE_ERROR("[OpenGL] Shaders' program linking error!");
			VIO_CORE_ERROR("{0}", infoLog.data());
			//Break the program
			VIO_CORE_ASSERT(false, "");
			return; //Exit the function
		}
		// Always detach shaders after a successful link.
		for (auto shaderID : glShaderIDs) {
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}
		m_programID = program;
	}

	//Assumes that shader is bound first
	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform4f(location, values.x, values.y, values.z, values.a);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform1i(location, value);
	}

}