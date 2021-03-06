#pragma once
#include "Violet/Events/Event.h"
namespace Violet {
	struct  WindowProperties {
		WindowProperties(const std::string& title ,uint32_t width , uint32_t height , bool VSync = true , const std::function<void(Event&)>& FUNCEventCallback = NULL)
			: m_title(title), m_width(width), m_height(height),m_VSync(VSync) , m_FUNCEventCallback(FUNCEventCallback){

		}

		std::string  m_title;
		uint32_t m_width;
		uint32_t m_height;
		bool m_VSync;
		std::function<void(Event&)> m_FUNCEventCallback;
	};

	//Interface for desktop window
	class VIOLET_API Window {
	public:
		virtual ~Window() = default;
		virtual void onUpdate() = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSyncEnabled() const = 0;
		virtual void* getNativeWindow() const = 0;

		virtual float getCurrentTime() const = 0;  //Semi-Temp, Preferable to be in a platform class

	public:
		static Scoped<Window> Create(const WindowProperties& properties);	//Has to be implemented in child per platform
	protected:
		virtual void init() = 0;
		virtual void shutDown() = 0;
	protected:
		WindowProperties m_properties;
		Window(const WindowProperties& properties) : m_properties( properties ) {}
	};



}