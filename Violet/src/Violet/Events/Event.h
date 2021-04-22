#pragma once
#define BIT(x) (1<<x)
namespace Violet{

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, //Window Events.

		AppTick, AppUpdate, AppRender,

		KeyPressed, KeyReleased, //Keyboard Events.
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, //Mouse Events.

		ItemsDropped
	};

	enum class EventCategory
	{
		None = 0,
		Application  = BIT(0),
		Input        = BIT(1),
		Keyboard     = BIT(2),
		Mouse        = BIT(3),
		MouseButton  = BIT(4)
	};


	class VIOLET_API Event {

	public:
		virtual ~Event() = default;
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		bool isInCategory(EventCategory category) {

			return getCategoryFlags()& (int)category;
		}
	public:
		bool handled = false;

	};


	class EventDispatcher {
		
	public:
		EventDispatcher(Event& event) :m_event(event) {}
		
		//FUNC will be deduced by the compiler
		template<typename T, typename FUNC>
		bool dispatch(const FUNC& FUNCEvent) {

			//If the event and function type matches.
			if (m_event.getEventType() == T::GetClassType()) {
				
				//Using or(|) instead of assignment(=) incase the dispatcher dispatched the same event type to multiple functions, once m_event handled status has been set to true by a one function's return, it will never be reset to false by the others
				m_event.handled |= FUNCEvent(static_cast<T&>(m_event));  //Call the appropriate function, Casting Event type to passed template type, Set m_handled.
				return true;
			}
			return false;
		}
	
	private:
		Event& m_event;
	};





}