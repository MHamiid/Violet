#pragma once
#include "Core.h"
namespace Violet {

	//Interface for desktop window
	class VIOLET_API Window {
	public:
		virtual ~Window() {}
		virtual void onUpdate() = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSyncEnabled() const = 0;

		static Window* Create(std::string title ,unsigned int width , unsigned int height);	//Has to be implemented in child per platform
	protected:
		virtual void init() = 0;
		virtual void shutDown() = 0;
	protected:
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool m_VSync = false;

		Window(std::string title, unsigned int width, unsigned int height): m_title(title) , m_width(width) , m_height(height){}

	};



}