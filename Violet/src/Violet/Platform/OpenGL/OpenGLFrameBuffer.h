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
		virtual void readColorAttachmentPixel(uint32_t colorAttachmentIndex, int xCoordinate, int yCoordinate, void* const outPixel) const override;
		virtual uint32_t getColorAttachmentID(uint32_t index = 0) const override { VIO_CORE_ASSERT(index < m_colorAttachmentsID.size(), "[Frame Buffer] Index Out Of Bounds!"); return m_colorAttachmentsID[index]; }
		virtual void clearColorAttachment(uint32_t colorAttachmentIndex, const void* clearValue) const override;
		virtual const FrameBufferSpecification& getSpecification() const override { return m_specification; }
	private:
		void validateSize();  //Recreate the FrameBuffer and it's attachments with the new size (m_specification.width, m_specification.height)
		static bool IsDepthFormat(FrameBufferTextureFormat format);
	private:
		uint32_t m_frameBufferID = 0;
		FrameBufferSpecification m_specification;   //Store the specification
		static const uint32_t s_MaxFrameBufferSize = 8192; //8k, TODO: Should query the GPU driver to find the max supported size

		/*Can have muliple color attachments but only one depth attachment*/
		std::vector<FrameBufferTextureSpecification> m_colorAttachmentSpecifications;
		FrameBufferTextureSpecification m_depthAttachmentSpecification;

		/*Texture IDs*/
		std::vector<uint32_t> m_colorAttachmentsID;
		uint32_t m_depthAttachmentID = 0;
	};


}