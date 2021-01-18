#pragma once
#include "Event.h"

namespace Violet {

	//Base class for key buttons events.
	class VIOLET_API KeyEvent : public Event {
	public:
		inline int getKeyCode() const { return m_keyCode; }

		//pure virtual functions implementation.
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input | (int)EventCategory::Keyboard; } //Assigning event to two categories.

	protected:
		KeyEvent(int keyCode) : m_keyCode(keyCode) {}
		int m_keyCode;

	};

	class VIOLET_API KeyPressedEvent : public KeyEvent {

	public:
		KeyPressedEvent(int keyCode , int repeatCount) :KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return m_repeatCount; }

		static EventType GetClassType() { return EventType::KeyPressed ; }
		//pure virtual functions implementation.
		virtual EventType getEventType() const override { return EventType::KeyPressed; }
		virtual const char* getName() const override { return "KeyPressed"; }

	private:
		int m_repeatCount;

	};

	class VIOLET_API KeyReleasedEvent : public KeyEvent {

	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
		
		static EventType GetClassType() { return EventType::KeyReleased; }
		//pure virtual functions implementation.
		virtual EventType getEventType() const override { return EventType::KeyReleased; }
		virtual const char* getName() const override { return "KeyReleased"; }
	};





}