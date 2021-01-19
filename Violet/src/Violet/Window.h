#pragma once
#include "Core.h"
namespace Violet {
	struct WindowProperties {
		WindowProperties(const std::string& title ,unsigned int width , unsigned int height , bool VSync = true)
			: m_title(title), m_width(width), m_height(height),m_VSync(VSync) { 

		}

		std::string  m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool m_VSync;
	};

	//Interface for desktop window
	class VIOLET_API Window {
	public:
		virtual ~Window() {}
		virtual void onUpdate() = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSyncEnabled() const = 0;


		static Window* Create(const WindowProperties& properties);	//Has to be implemented in child per platform
	protected:
		virtual void init() = 0;
		virtual void shutDown() = 0;
	protected:
		WindowProperties m_properties;
		Window(const WindowProperties& properties) : m_properties( properties ) {}
	};



}