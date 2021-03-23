#pragma once
#include "Violet/Renderer/Buffer.h"
namespace Violet {

	class VIOLET_API OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual void setData(const void* data, uint32_t size) const override;
		virtual const VertexLayout& getLayout() const override { return m_vertexLayout; }
		virtual void setLayout(const VertexLayout& layout) override { m_vertexLayout = layout; }
	private:
		uint32_t m_vertexBufferID;
		VertexLayout m_vertexLayout;
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