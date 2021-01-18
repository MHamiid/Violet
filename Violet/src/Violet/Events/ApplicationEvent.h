#pragma once
#include "Event.h"


namespace Violet {

	class VIOLET_API WindowResizeEvent : public Event {

	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width) , m_height(height){}

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		static EventType GetClassType() { return EventType::WindowResize; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; } //Assigning event to one category.
		virtual EventType getEventType() const override { return EventType::WindowResize; }
		virtual const char* getName() const override { return "WindowResize"; }


	private:
		unsigned int m_width, m_height;

	};


	class VIOLET_API WindowCloseEvent : public Event {

	public:
		WindowCloseEvent() {}

		static EventType GetClassType() { return EventType::WindowClose; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; } //Assigning event to one category.
		virtual EventType getEventType() const override { return EventType::WindowClose; }
		virtual const char* getName() const override { return "WindowClose"; }

	};



}