#include "VIOPCH.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Violet {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) : m_programID(0) //initialize with 0
	{

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			VIO_CORE_ERROR("[OpenGL] Vertex shader compilation error!");
			VIO_CORE_ERROR("{0}", infoLog.data());
			//Break the program
			VIO_CORE_ASSERT(false,"");
			return; //Exit the function
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			VIO_CORE_ERROR("[OpenGL] Fragment shader compilation error!");
			VIO_CORE_ERROR("{0}", infoLog.data());
			//Break the program
			VIO_CORE_ASSERT(false, "");
			return; //Exit the function
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_programID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);

		// Link our program
		glLinkProgram(m_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_programID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			VIO_CORE_ERROR("[OpenGL] Shaders' program linking error!");
			VIO_CORE_ERROR("{0}", infoLog.data());
			//Break the program
			VIO_CORE_ASSERT(false, "");
			return; //Exit the function
		}
		// Always detach shaders after a successful link.
		glDetachShader(m_programID, vertexShader);
		glDetachShader(m_programID, fragmentShader);
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

}