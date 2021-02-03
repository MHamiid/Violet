#include "VIOPCH.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>
namespace Violet {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path): m_path(path)
	{
		//Reading the image
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); //Set the image to be flipped to match OpenGL format
		stbi_uc* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VIO_CORE_ASSERT(imageData, "[OPENGL TEXTURE] Stbi Failed To Load Image!");
		
		m_width = width;
		m_height = height;

		//Setting up the texture
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);  //Get an id   //For OpenGL 4.5
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glTextureStorage2D(m_textureID, 1, GL_RGB8, m_width, m_height); //Allocate the required memory on the GPU   //For OpenGL 4.5

		//Set some few texture parameters
		//glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //For OpenGL 4.5
		//glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //For OpenGL 4.5
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		//glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, imageData); //Upload the texture  //For OpenGL 4.5
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);  //Free memory

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_textureID);
	}	
	void OpenGLTexture2D::bind(uint32_t textureSlot) const
	{
		//glBindTextureUnit(textureSlot, m_textureID);   //For OpenGL 4.5
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
}