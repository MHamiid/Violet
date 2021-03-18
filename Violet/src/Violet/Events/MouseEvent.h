#pragma once
#include "Event.h"

namespace Violet {

	class VIOLET_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x) , m_mouseY(y){}
		
		float getX() const { return m_mouseX; }
		float getY() const { return m_mouseY; }

		static EventType GetClassType() { return EventType::MouseMoved; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input | (int)EventCategory::Mouse; } //Assigning event to two categories.
		virtual EventType getEventType() const override { return EventType::MouseMoved; }
		virtual const char* getName() const override { return "MouseMoved"; }


	private:
		float m_mouseX, m_mouseY;
	};

	class VIOLET_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_offsetX(xOffset), m_offsetY(yOffset) {}

		float getXOffset() const { return m_offsetX; }
		float getYOffset() const { return m_offsetY; }

		static EventType GetClassType() { return EventType::MouseScrolled; }
		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input | (int)EventCategory::Mouse; } //Assigning event to two categories.
		virtual EventType getEventType() const override { return EventType::MouseScrolled; }
		virtual const char* getName() const override { return "MouseScrolled"; }


	private:
		float m_offsetX, m_offsetY;
	};


	//Base class for mouse buttons events.
	class VIOLET_API MouseButtonEvent :public Event {
	public:
		int getMouseButton() const { return m_button; }

		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input | (int)EventCategory::Mouse | (int)EventCategory::MouseButton; } //Assigning event to three categories.

	protected:
		MouseButtonEvent(int button) :m_button(button) {}
		int m_button;
	};


	class VIOLET_API MouseButtonPressedEvent : public MouseButtonEvent {

	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button){}

		static EventType GetClassType() { return EventType::MouseButtonPressed; }
		//pure virtual functions implementation.
		virtual EventType getEventType() const override { return EventType::MouseButtonPressed; }
		virtual const char* getName() const override { return "MouseButtonPressed"; }



	};


	class VIOLET_API MouseButtonReleasedEvent : public MouseButtonEvent {

	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		static EventType GetClassType() { return EventType::MouseButtonReleased; }
		//pure virtual functions implementation.
		virtual EventType getEventType() const override { return EventType::MouseButtonReleased; }
		virtual const char* getName() const override { return "MouseButtonReleased"; }



	};

}