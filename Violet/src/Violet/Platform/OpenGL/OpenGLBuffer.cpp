#include "VIOPCH.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Violet {
	/*
		//////////////////////////////////////////////////////////////
		//////////////////Vertex Buffer Implementation///////////////	
		////////////////////////////////////////////////////////////
	*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) 
	{	
		//glCreateBuffers(1, &m_vertexBufferID);
		glGenBuffers(1, &m_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_vertexBufferID);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	}

	void OpenGLVertexBuffer::unBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
		//////////////////////////////////////////////////////////////
		//////////////////Index Buffer Implementation////////////////
		////////////////////////////////////////////////////////////
	*/
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_count(count)
	{
		//glCreateBuffers(1, &m_indexBufferID);
		glGenBuffers(1, &m_indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(UINT32), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_indexBufferID);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	}

	void OpenGLIndexBuffer::unBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::getCount() const
	{
		return m_count;
	}

}