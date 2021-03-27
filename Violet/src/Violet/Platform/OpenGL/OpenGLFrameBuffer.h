#pragma once
#include "Violet/Renderer/FrameBuffer.h"

namespace Violet {

	class VIOLET_API OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual uint32_t getColorAttachmentID() const override { return m_colorAttachmentID; }
		virtual const FrameBufferSpecification& getSpecification() const override { return m_specification; }
	public:
		void invalidate();  //Invalidate the FrameBuffer state
	private:
		uint32_t m_frameBufferID = 0;
		uint32_t m_colorAttachmentID = 0, m_depthAttachmentID = 0; //Textures
		FrameBufferSpecification m_specification;   //Store the specification

	};


}