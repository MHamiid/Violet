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
		virtual void addVertexBufferAndLinkLayout(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;
	public:
		static Ref<VertexArray> Create();
	};

}