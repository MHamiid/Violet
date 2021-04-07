#pragma once
#include "Event.h"


namespace Violet {

	class VIOLET_API WindowResizeEvent : public Event {

	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width) , m_height(height){}

		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }

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
		WindowCloseEvent() = default;

		static EventType GetClassType() { return EventType::WindowClose; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; } //Assigning event to one category.
		virtual EventType getEventType() const override { return EventType::WindowClose; }
		virtual const char* getName() const override { return "WindowClose"; }

	};

	class VIOLET_API ItemsDroppedEvent : public Event {

	public:
		ItemsDroppedEvent(int count, const char** itemsPaths) : m_count(count), m_itemspaths(itemsPaths){};

		int getItemsCount() { return m_count; }
		const char** getItemsPaths() const { return m_itemspaths; }

		static EventType GetClassType() { return EventType::ItemsDropped; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; } //Assigning event to one category.
		virtual EventType getEventType() const override { return EventType::ItemsDropped; }
		virtual const char* getName() const override { return "ItemsDropped"; }
	private:
		int m_count;
		const char** m_itemspaths;
	};

}