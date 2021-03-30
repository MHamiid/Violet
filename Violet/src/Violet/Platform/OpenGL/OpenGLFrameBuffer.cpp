#include "VIOPCH.h"
#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>

namespace Violet {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification) : m_specification(specification)
	{
		invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_frameBufferID);
		//Delete attachments
		glDeleteTextures(1, &m_colorAttachmentID);
		glDeleteTextures(1, &m_depthAttachmentID);
	}

	void OpenGLFrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
		//Update the viewport incase the FrameBuffer size has changed
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFrameBuffer::unBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height)
	{
		//Check if the new size is valid
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize) {
			VIO_CORE_WARN("[OpenGL FrameBuffer] Attempted To Resize The FrameBuffer To {0}, {1}", width, height);
			return;  //If not valid return from function
		}
		//Update the new size specification
		m_specification.width = width;
		m_specification.height = height;

		//Invalidate the specification
		invalidate();
	}

	void OpenGLFrameBuffer::invalidate()
	{

		if (m_frameBufferID) //If frame buffer is set
		{ 
			glDeleteFramebuffers(1, &m_frameBufferID);
			//Delete attachments
			glDeleteTextures(1, &m_colorAttachmentID);
			glDeleteTextures(1, &m_depthAttachmentID);
		}



		//glCreateFramebuffers(1, &m_frameBufferID);  //For OpenGL 4.5
		glGenFramebuffers(1, &m_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

		/*Create a color attachment*/
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentID);  //Get an id   //For OpenGL 4.5
		glGenTextures(1, &m_colorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		/*
		* Set some few texture parameters
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Attach the color texture to the frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachmentID, 0);



		/*Create a depth attachment*/
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentID);  //Get an id   //For OpenGL 4.5
		glGenTextures(1, &m_depthAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachmentID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height)  (Texture storage can't be sampled from)  //For OpenGL 4.5
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);  /*GL_UNSIGNED_INT_24_8, the first 24 bit for the depth buffer,
																																								      the next 8 bits for STENCIL buffer*/


		//Attach the color texture to the frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachmentID, 0);


		//Check the status
		VIO_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "[Frame Buffer] Frame Buffer Incomplete Status!");

		//UnBind the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

}