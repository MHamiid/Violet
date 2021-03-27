#pragma once

namespace Violet {
	//Defines the FrameBuffer
	struct FrameBufferSpecification {

		uint32_t width = 0, height = 0;

		bool swapChainTarget = false; //For vulkan support later on (true: that's an off screen buffer, false: that's an on screen buffer),
									  //(ture OpenGL equivalent: glBindFramebuffer(0), Don't have a FrameBuffer, render to the screen)
	};

	/*Interface*/
	class VIOLET_API FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual uint32_t getColorAttachmentID() const = 0;   //To retrieve the frame buffer texture and use it to render and sample from
		virtual const FrameBufferSpecification& getSpecification() const = 0;
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);

	};

}
