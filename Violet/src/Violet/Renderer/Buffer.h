#pragma once

namespace Violet {

	enum class VertexAttributeDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	
	static uint32_t GetVertexAttributeDataTypeSize(VertexAttributeDataType type){
		
		switch (type) {
	
			case VertexAttributeDataType::Float:    return sizeof(float);
			case VertexAttributeDataType::Float2:   return sizeof(float) * 2;
			case VertexAttributeDataType::Float3:   return sizeof(float) * 3;
			case VertexAttributeDataType::Float4:   return sizeof(float) * 4;
			case VertexAttributeDataType::Mat3:     return sizeof(float) * 3 * 3;
			case VertexAttributeDataType::Mat4:     return sizeof(float) * 4 * 4;
			case VertexAttributeDataType::Int:      return sizeof(int);
			case VertexAttributeDataType::Int2:     return sizeof(int) * 2;
			case VertexAttributeDataType::Int3:     return sizeof(int) * 3;
			case VertexAttributeDataType::Int4:     return sizeof(int) * 4;
			case VertexAttributeDataType::Bool:     return sizeof(bool);

		}
		VIO_CORE_ASSERT(false, "[Buffer] VertexAttributeDataType Selection Failed!");
		return 0;
	}

	struct VIOLET_API VertexAttribute {
		VertexAttribute() = default;
		VertexAttribute(VertexAttributeDataType attributeDataType, const std::string& attributeName, bool attributeNormalized = false) : dataType(attributeDataType), name(attributeName),
			size(GetVertexAttributeDataTypeSize(attributeDataType)), offset(0), normalized(attributeNormalized){}
		uint32_t getDataTypeCount() const {

			switch (dataType) {

			case VertexAttributeDataType::Float:    return 1;
			case VertexAttributeDataType::Float2:   return 2;
			case VertexAttributeDataType::Float3:   return 3;
			case VertexAttributeDataType::Float4:   return 4;
			case VertexAttributeDataType::Mat3:     return 3; // 3 * Float3
			case VertexAttributeDataType::Mat4:     return 4; // 4 * Float4
			case VertexAttributeDataType::Int:      return 1;
			case VertexAttributeDataType::Int2:     return 2;
			case VertexAttributeDataType::Int3:     return 3;
			case VertexAttributeDataType::Int4:     return 4;
			case VertexAttributeDataType::Bool:     return 1;

			}
			VIO_CORE_ASSERT(false, "[Buffer] VertexAttributeDataType Selection Failed!");
			return 0;
		
		}

		VertexAttributeDataType dataType;
		std::string name;  //It has no usage, mostly used for clarity when developing, and also for consistency in the future, as in DirectX and HLSL it has a usage
		uint32_t size;
		size_t offset;  //Set to size_t cuz it's converted to (const void*) later on in the OpenGL implementation
		bool normalized;
	};
	
	class VIOLET_API VertexLayout {
	public:
		VertexLayout() = default;
		VertexLayout(std::initializer_list<VertexAttribute> layoutAttributes) : m_vertexAttributes(layoutAttributes), m_stride(0){
			size_t offset = 0;
			for (VertexAttribute& attribute : m_vertexAttributes) {
				//Set the offset for each vertex attribute in the vector
				attribute.offset = offset;
				//Increment the offset to the next vertex attribute
				offset += attribute.size;
				//Keep track of the whole vertex size
				m_stride += attribute.size;
			}
				
		}
		const std::vector<VertexAttribute>& getLayoutAttributes() const { return m_vertexAttributes; }
		uint32_t getStride() const { return m_stride; }
	private:
		std::vector<VertexAttribute> m_vertexAttributes;
		uint32_t m_stride;

	};


	/*
	Interface
	*/
	class VIOLET_API VertexBuffer{
	public:
		virtual ~VertexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual void setData(const void* data, uint32_t size) const = 0;
		virtual const VertexLayout& getLayout() const = 0;
		virtual void setLayout(const VertexLayout& layout) = 0;
	public:
		static Ref<VertexBuffer> Create(uint32_t size);  //For dynamic buffer
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	/*
	Interface
	NOTE: Currently using 32-bit per index only
	*/
	class VIOLET_API IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual uint32_t getCount() const = 0;
	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	
	};

}