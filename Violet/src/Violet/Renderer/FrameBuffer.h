#pragma once

namespace Violet {

	//Defines the FrameBuffer's texture format
	enum class FrameBufferTextureFormat {

		NONE = 0,

		/*Color*/
		RGBA8,
		RED_INTEGER,  //Just one channel as an integer

		/*Depth/Stencil*/
		DEPTH24STENCIL8,

		/*Defaults*/
		DEPTH = DEPTH24STENCIL8

	};

	//Defines the FrameBuffer's texture specification
	struct FrameBufferTextureSpecification {

		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format) : textureFormat(format) {}
		 
		FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::NONE;

		/*TODO: Add filtering and wrapping*/
	};

	//Defines all the textures attached to the FrameBuffer
	struct FrameBufferAttachmentSpecification {

		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: textureAttachmentsSpecification(attachments) {}

		std::vector<FrameBufferTextureSpecification> textureAttachmentsSpecification;
	};

	//Defines the FrameBuffer
	struct FrameBufferSpecification {

		uint32_t width = 0, height = 0;
		FrameBufferAttachmentSpecification textureAttachmentsSpecification;


		bool swapChainTarget = false; //For vulkan support later on (true: that's an off screen buffer, false: that's an on screen buffer),
									  //(ture OpenGL equivalent: glBindFramebuffer(0), Don't have a FrameBuffer, render to the screen)
	};

	/*Interface*/
	class VIOLET_API FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual void resize(uint32_t width, uint32_t height) = 0;
		virtual void readColorAttachmentPixel(uint32_t colorAttachmentIndex, int xCoordinate, int yCoordinate, void* const outPixel) const = 0;
		virtual uint32_t getColorAttachmentID(uint32_t index = 0) const = 0;   //To retrieve the frame buffer texture and use it to render and sample from
		virtual const FrameBufferSpecification& getSpecification() const = 0;
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);

	};

}
