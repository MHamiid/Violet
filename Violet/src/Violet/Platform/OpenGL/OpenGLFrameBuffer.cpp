#include "VIOPCH.h"
#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>

namespace Violet {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification) : m_specification(specification)
	{
		/*Can have muliple color attachments but only one depth attachment*/
		for (auto textureSpecification : m_specification.textureAttachmentsSpecification.textureAttachmentsSpecification)
		{
			if (textureSpecification.textureFormat != FrameBufferTextureFormat::NONE)
			{
				if (!IsDepthFormat(textureSpecification.textureFormat))
				{
					m_colorAttachmentSpecifications.emplace_back(textureSpecification);
				}
				else
				{
					m_depthAttachmentSpecification = textureSpecification;
				}
			}
		}
		validateSize();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_frameBufferID);
		//Delete attachments
		glDeleteTextures((GLsizei)m_colorAttachmentsID.size(), m_colorAttachmentsID.data());
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
		validateSize();
	}

	/*Recreate the FrameBuffer and it's attachments with the new size (m_specification.width, m_specification.height)*/
	void OpenGLFrameBuffer::validateSize()
	{
		/*Frame Buffer Creation*/
		if (m_frameBufferID) //If frame buffer is set
		{ 
			glDeleteFramebuffers(1, &m_frameBufferID);
			//Delete attachments
			glDeleteTextures((GLsizei)m_colorAttachmentsID.size(), m_colorAttachmentsID.data());
			glDeleteTextures(1, &m_depthAttachmentID);

			//Clear the deleted textures ids
			m_colorAttachmentsID.clear();
			m_depthAttachmentID = 0;
		}

		//glCreateFramebuffers(1, &m_frameBufferID);  //For OpenGL 4.5
		glGenFramebuffers(1, &m_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

		/*Attachments*/

		/*Create the color attachments*/

		if (m_colorAttachmentSpecifications.size())  //If there is any color attachment
		{
			m_colorAttachmentsID.resize(m_colorAttachmentSpecifications.size()); //Set the size of the colorAttachmentsID vector to be the size of number of the color attachments vector

			//glCreateTextures(GL_TEXTURE_2D, m_colorAttachmentsID.size(), m_colorAttachmentsID.data());  //Get an id   //For OpenGL 4.5
			glGenTextures((GLsizei)m_colorAttachmentsID.size(), m_colorAttachmentsID.data());
		
			for (size_t i = 0; i < m_colorAttachmentsID.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_colorAttachmentsID[i]);
				GLuint internalFormat;
				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					internalFormat = GL_RGBA8;
					break;
				default:
					VIO_CORE_ASSERT(false, "[Frame Buffer] Color Attachment Format Selection Failed!");
					break;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				/*TODO: Should get the filtering and wrapping parameters from FrameBufferTextureSpecification when it's added*/

				/*
				* Set some few texture parameters
				*/
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				//Attach the color texture to the frame buffer
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D, m_colorAttachmentsID[i], 0);
			}
		}

		
		if (m_depthAttachmentSpecification.textureFormat != FrameBufferTextureFormat::NONE)
		{
			/*Create a depth attachment*/
			//glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentID);  //Get an id   //For OpenGL 4.5
			glGenTextures(1, &m_depthAttachmentID);
			glBindTexture(GL_TEXTURE_2D, m_depthAttachmentID);


			GLuint internalFormat;
			GLenum type;
			switch (m_depthAttachmentSpecification.textureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				internalFormat = GL_DEPTH24_STENCIL8;
				type = GL_UNSIGNED_INT_24_8;
				break;
			default:
				VIO_CORE_ASSERT(false, "[Frame Buffer] Depth Attachment Format Selection Failed!");
				break;
			}

			//glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, m_specification.width, m_specification.height)  (Texture storage can't be sampled from)  //For OpenGL 4.5
			/*GL_UNSIGNED_INT_24_8, the first 24 bit for the depth buffer, the next 8 bits for STENCIL buffer*/
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, type, nullptr);

			/*TODO: Should get the filtering and wrapping parameters from FrameBufferTextureSpecification when it's added*/

			/*
			* Set some few texture parameters
			*/
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		    //Attach the depth texture to the frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachmentID, 0);
		}

		/*End Attachments*/

		/*specify which color buffers are to be drawn into*/
		if (m_colorAttachmentsID.size() > 1)
		{
			VIO_CORE_ASSERT(m_colorAttachmentsID.size() <= 4, "[Frame Buffer] Can't Have More Than Four Color Buffers");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_colorAttachmentsID.size(), buffers);
		}
		else if (m_colorAttachmentsID.empty())
		{
			//Only depth-pass
			glDrawBuffer(GL_NONE);
		}


		//Check the status
		VIO_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "[Frame Buffer] Frame Buffer Incomplete Status!");

		//UnBind the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

	}

	bool OpenGLFrameBuffer::IsDepthFormat(FrameBufferTextureFormat format)
	{
		/*NOTE: Added a switch case when a new depth format is added to the FrameBufferTextureFormat enum class*/
		switch (format)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		
		return false;
	}

}