#pragma once
#include "Buffer.h"

namespace Violet {
	/*
	Interface
	*/
	class VIOLET_API VertexArray {
	public:
		virtual ~VertexArray() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual void addVertexBufferAndLinkLayout(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;
	public:
		static VertexArray* Create();
	};

}