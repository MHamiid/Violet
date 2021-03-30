#pragma once
#include "Violet/Renderer/FrameBuffer.h"

namespace Violet {

	class VIOLET_API OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual void resize(uint32_t width, uint32_t height) override;
		virtual uint32_t getColorAttachmentID() const override { return m_colorAttachmentID; }
		virtual const FrameBufferSpecification& getSpecification() const override { return m_specification; }
	public:
		void invalidate();  //Invalidate the FrameBuffer state
	private:
		uint32_t m_frameBufferID = 0;
		uint32_t m_colorAttachmentID = 0, m_depthAttachmentID = 0; //Textures
		FrameBufferSpecification m_specification;   //Store the specification
		static const uint32_t s_MaxFrameBufferSize = 8192; //8k, TODO: Should query the GPU driver to find the max supported size

	};


}