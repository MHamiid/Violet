#pragma once
#include "Violet/Renderer/VertexArray.h"
namespace Violet {

	class VIOLET_API OpenGLVertexArray : public VertexArray{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual void addVertexBufferAndLinkLayout(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return m_vertexBuffers; }
		virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return m_indexBuffer; }
	private:
		uint32_t m_vertexArrayID;
		uint32_t m_lastEnabledVertexAttribute = 0;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;
	};


}