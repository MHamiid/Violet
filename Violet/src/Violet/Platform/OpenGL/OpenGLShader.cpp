#include "VIOPCH.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <filesystem>
namespace Violet {
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_programID(0) //initialize with 0
	{
		//Parse the file content
		std::unordered_map<GLenum, std::string> shaderSources = parseShadersFile(filePath);

		//Extract the shaderName from the filePath
		std::filesystem::path path = filePath;
		m_shaderName = path.stem().string();

		createProgram(shaderSources);


		//###IF WE ARE NOT USING filesystem(C++17)
		/*
			//@example assets/shaders/shader_name.glsl  ===> shader_name

			//Find the last slash if exists
			size_t lastSlashPos = filePath.find_last_of("/\\"); //meaning last of forward slash or back slash

			if (lastSlashPos == std::string::npos) { lastSlashPos = 0; } //If the filePath doesn't contain slashes @example shader_name.glsl,
																		 //We set the position to point at the first char of the file name
																		 //@example shader_name.glsl ===> indexOf(T)
			else { lastSlashPos = lastSlashPos + 1; }  //Points to the first char of the file name
													   //@example assets/shaders/shader_name.glsl  ===> indexOf(T)

			//Find the last dot if exists
			size_t lastDotPos = filePath.rfind('.'); //Find the last dot

			size_t shaderNameCharCount = lastDotPos == std::string::npos ? filePath.size() - lastSlashPos : lastDotPos - lastSlashPos;	//[If] the filePath doesn't contain dots @example  assets/shaders/shader_name,
																																		//We set the count to be the number of chars from lastSlashPos to the end
																																		//[Else] we set the count to be the number of chars between the dot and the slash

			m_shaderName = filePath.substr(lastSlashPos, shaderNameCharCount); //Note: lastSlashPos points to the first char of the file name
		*/
		VIO_CORE_DEBUG("[OpenGL Shader] Loaded Shader '{0}' From File", m_shaderName);


	}
	OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& filePath) : m_shaderName(shaderName)
	{
		//Parse the file content
		std::unordered_map<GLenum, std::string> shaderSources = parseShadersFile(filePath);
		createProgram(shaderSources);

		VIO_CORE_DEBUG("[OpenGL Shader] Loaded Shader '{0}' From File With It's Provided Name", m_shaderName);
	}
	OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc) : m_programID(0) //initialize with 0
		, m_shaderName(shaderName)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		createProgram(shaderSources);

		VIO_CORE_DEBUG("[OpenGL Shader] Loaded Shader '{0}' From Strings With It's Provided Name", m_shaderName);
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

	//Assumes that shader is bound first
	void OpenGLShader::setInt(const std::string& name, int value)
	{
		uploadUniformInt(name, value);
	}
	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		uploadUniformIntArray(name, values, count);
	}
	void OpenGLShader::setFloat(const std::string& name, float value)
	{
		uploadUniformFloat(name, value);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& values)
	{
		uploadUniformFloat4(name, values);
	}
	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix)
	{
		uploadUniformMat4(name, matrix);
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
		std::ifstream stream(filePath, std::ios::in | std::ios::binary);
		VIO_CORE_ASSERT(stream, "[OpenGL Shader]Couldn't Open Shaders File!");
		if (stream) {
			std::string line;
			while (getline(stream, line)) {
				//Split the shader source code into Vertex Shader and Fragment Shader
				if (line.find("#type") != std::string::npos) {
					if (!buffer.empty()) {
						shaderSourceCodes[static_cast<GLenum>(type)] = buffer;
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

			VIO_CORE_ASSERT(!(type == ShaderType::NONE), "[OpenGL Shader] Couldn't Read Shaders!");
			shaderSourceCodes[static_cast<GLenum>(type)] = buffer;
		}


		return shaderSourceCodes;
	}

	void OpenGLShader::createProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Get a program object.
		GLuint program = glCreateProgram();
		constexpr size_t maxNumberOfShadersAllowed = 10;  //Defines the storage to allocate
		VIO_CORE_ASSERT((shaderSources.size() <= maxNumberOfShadersAllowed), "[OpenGL Shader] glShaderIDs Array Doesn't Have Enough Storage!");
		std::array<GLenum, maxNumberOfShadersAllowed> glShaderIDs;
		int indexOfLastFreeStorage = 0; //Points to the next free space
		for (auto&& [shaderType, shaderSourceCode] : shaderSources) {

			GLuint shader = glCreateShader(shaderType);
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = reinterpret_cast<const GLchar*>(shaderSourceCode.c_str());
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
				VIO_CORE_ERROR("[OpenGL Shader] {0} Shader Compilation Error In '{1}'!", shaderTypeToString(shaderType), m_shaderName);
				VIO_CORE_ERROR("{0}", infoLog.data());
				//Break the program
				VIO_CORE_ASSERT(false, "");
				return; //Exit the function
			}
			// shader is successfully compiled.
			// Now time to link them together into a program.
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[indexOfLastFreeStorage++] = (shader);
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
			for (int i = 0; i < indexOfLastFreeStorage; i++) {
				glDeleteShader(glShaderIDs[i]);
			}

			// Use the infoLog as you see fit.
			VIO_CORE_ERROR("[OpenGL Shader] Shaders' Program Linking Error In '{0}'!", m_shaderName);
			VIO_CORE_ERROR("{0}", infoLog.data());
			//Break the program
			VIO_CORE_ASSERT(false, "");
			return; //Exit the function
		}
		// Always detach shaders after a successful link.
		for (int i = 0; i < indexOfLastFreeStorage; i++) {
			glDetachShader(program, glShaderIDs[i]);
			glDeleteShader(glShaderIDs[i]);
		}
		m_programID = program;
	}

	//Assumes that shader is bound first
	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform1iv(location, count, values);
	}
	void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniform4f(location, values.x, values.y, values.z, values.a);
	}
	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		//TODO: cache uniform location
		GLint location = glGetUniformLocation(m_programID, name.c_str());
		//If you want to Check if the retrieved location is valid
		if (location == -1) VIO_CORE_WARN("[OpenGL Shader] Uniform {0} Not Found Or Maybe Is Not Used!", name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::shaderTypeToString(GLenum shaderType)
	{
		std::string shaderTypeString = "Undefined";
		switch (shaderType)
		{
			case GL_VERTEX_SHADER:     shaderTypeString = "Vertex";     break;
			case GL_FRAGMENT_SHADER:   shaderTypeString = "Fragment";   break;
		}

		return shaderTypeString;
	}


}