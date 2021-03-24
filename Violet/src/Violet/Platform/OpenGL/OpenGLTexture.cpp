#include "VIOPCH.h"
#include "OpenGLTexture.h"
#include <stb_image.h>
namespace Violet {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height)
	{
		/*
		* NOTE: Formats are hardcoded, should be parameterized
		*/
		m_internalFormat = GL_RGBA8;  //How shaders are gonna sample from it
		m_dataFormat = GL_RGBA;
		//Setting up the texture
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);  //Get an id   //For OpenGL 4.5
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glTextureStorage2D(m_textureID, 1, internalFormat, m_width, m_height); //Allocate the required memory on the GPU   //For OpenGL 4.5

		/*
		* Set some few texture parameters
		* NOTE: Some texture parameters are hardcoded, should be parameterized
		*/
		//glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //For OpenGL 4.5
		//glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //For OpenGL 4.5
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		/*
		Set to repeat the texture if we exceeded the texture coordinates range [0.0f, 1.0f]
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path): m_path(path)
	{
		//Reading the image
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); //Set the image to be flipped to match OpenGL format
		stbi_uc* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VIO_CORE_ASSERT(imageData, "[OPENGL TEXTURE] Stbi Failed To Load Image!");
		
		m_width = width;
		m_height = height;

		//Identify the format based on the number of the channels
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }
		else if (channels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
		VIO_CORE_ASSERT(internalFormat && dataFormat, "[OPENGL TEXTURE] Format Is Not Set!");

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		//Setting up the texture
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);  //Get an id   //For OpenGL 4.5
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glTextureStorage2D(m_textureID, 1, internalFormat, m_width, m_height); //Allocate the required memory on the GPU   //For OpenGL 4.5

		/*
		* Set some few texture parameters
		* NOTE: Some texture parameters are hardcoded, should be parameterized
		*/
		//glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //For OpenGL 4.5
		//glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //For OpenGL 4.5
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		/*
		Set to repeat the texture if we exceeded the texture coordinates range [0.0f, 1.0f]
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		//glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, imageData); //Upload the texture  //For OpenGL 4.5
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);  //Free memory

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_textureID);
	}	
	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		/*
		* NOTE: size could be used with glTexImage2D to set a sub region of the texture to be used instead of using the whole texture,
		* Currently size is used to assert that the size of the data is matching with the size of the defined texture.
		*/
		uint32_t bytesPerPixel = m_dataFormat == GL_RGBA ? 4 : 3;  //else { m_dataFormat = GL_RGB}
		VIO_CORE_ASSERT(size == m_width * m_height * bytesPerPixel, "[OpenGL Texture] Data size doesn't match with the provided texture size");
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::bind(uint32_t textureSlot) const
	{
		//glBindTextureUnit(textureSlot, m_textureID);   //For OpenGL 4.5
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
}