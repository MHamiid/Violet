#pragma once
#include "Violet/Renderer/Buffer.h"
namespace Violet {

	class VIOLET_API OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		virtual void bind() const override;
		virtual void unBind() const override;
	private:
		uint32_t m_vertexBufferID;
	};

	class VIOLET_API OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual uint32_t getCount() const override;
	private:
		uint32_t m_indexBufferID;
		uint32_t m_count;
	};

}