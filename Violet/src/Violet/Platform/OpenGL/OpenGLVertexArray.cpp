#include "VIOPCH.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
namespace Violet {
	
	 static GLenum VertexAttributeDataTypeToOpenGLType(VertexAttributeDataType type) {

		switch (type) {

		case VertexAttributeDataType::Float:    return GL_FLOAT;
		case VertexAttributeDataType::Float2:   return GL_FLOAT;
		case VertexAttributeDataType::Float3:   return GL_FLOAT;
		case VertexAttributeDataType::Float4:   return GL_FLOAT;
		case VertexAttributeDataType::Mat3:     return GL_FLOAT;
		case VertexAttributeDataType::Mat4:     return GL_FLOAT;
		case VertexAttributeDataType::Int:      return GL_INT;
		case VertexAttributeDataType::Int2:     return GL_INT;
		case VertexAttributeDataType::Int3:     return GL_INT;
		case VertexAttributeDataType::Int4:     return GL_INT;
		case VertexAttributeDataType::Bool:     return GL_BOOL;

		}
		VIO_CORE_ASSERT(false, "[Buffer] VertexAttributeDataType Selection Failed!");
		return 0;
	}
	/*
		 /////////////////////////////////////////////////////////////
		//////////////////Vertex Array Implementation////////////////	
	   /////////////////////////////////////////////////////////////
	*/

	OpenGLVertexArray::OpenGLVertexArray()
	{
		//glCreateVertexArrays(1, &m_vertexArrayID);
		glGenVertexArrays(1, &m_vertexArrayID);

	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{

		glDeleteVertexArrays(1, &m_vertexArrayID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_vertexArrayID);

	}

	void OpenGLVertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBufferAndLinkLayout(const Ref<VertexBuffer>& vertexBuffer)
	{
		const std::vector<VertexAttribute>& attributes = vertexBuffer->getLayout().getLayoutAttributes();
		//Check if the attributes vector is not equal to zero
		VIO_CORE_ASSERT(attributes.size(), "No Layout Has Been Set For Vertex Buffer!");


		//Bind the vertex array
		glBindVertexArray(m_vertexArrayID);
		vertexBuffer->bind();

		//Set the layout for the buffer
		for (int i = 0; i < attributes.size(); i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attributes[i].getDataTypeCount(),
				VertexAttributeDataTypeToOpenGLType(attributes[i].dataType),
				attributes[i].normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->getLayout().getStride(),
				(const void*)attributes[i].offset);
		}

		//Store the vertex buffer into the vertex buffers vector
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		//Bind the vertex array
		glBindVertexArray(m_vertexArrayID);
		indexBuffer->bind();	

		//Store the index buffer
		m_indexBuffer = indexBuffer;
	}

}