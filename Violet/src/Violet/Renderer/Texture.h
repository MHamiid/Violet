#pragma once

namespace Violet {
	/*
	 Interface for different type textures
	*/
	class VIOLET_API Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t textureSlot = 0) const = 0;
	};

	/*
	  Interface 
	*/
	class VIOLET_API Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}