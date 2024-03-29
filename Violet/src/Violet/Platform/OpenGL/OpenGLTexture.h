#pragma once
#include "Violet/Renderer/Texture.h"
#include <glad/glad.h>

namespace Violet {

	class VIOLET_API OpenGLTexture2D : public Texture2D{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return m_width; };
		virtual uint32_t getHeight() const override { return m_height; };
		virtual uint32_t getTextureID() const override { return m_textureID; }

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t textureSlot = 0) const override;

		virtual bool operator==(const Texture& other) const override { return m_textureID == ((OpenGLTexture2D&)other).m_textureID; }
	private:
		std::string m_path;
		uint32_t m_width, m_height;
		uint32_t m_textureID;
		GLenum m_internalFormat, m_dataFormat;
	};

}