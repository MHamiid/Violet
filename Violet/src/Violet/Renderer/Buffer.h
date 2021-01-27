#pragma once

namespace Violet {

	/*
	Interface
	*/
	class VIOLET_API VertexBuffer{
	public:
		virtual ~VertexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
	public:
		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	/*
	Interface
	*/
	class VIOLET_API IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual uint32_t getCount() const = 0;
	public:
		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	
	};

}