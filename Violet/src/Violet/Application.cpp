#include "Application.h"
#include "Log.h"
#include "Events/MouseEvent.h"
namespace Violet {
	bool eventHandling(MouseButtonPressedEvent& ev) {
		VIO_INFO("Event Handle Function Called");
		return true;
	}
	Application::Application()
	{
	}
	Application::~Application()
	{

	}
	void Application::run()
	{	

		MouseButtonPressedEvent e(30);
		VIO_DEBUG(e.getName());
		VIO_TRACE("Is Event Handled: {}",e.getEventHandleStatus());
	/*	VIO_DEBUG(e.getWidth());
		VIO_DEBUG(e.getHeight());*/
		VIO_DEBUG((int)e.getEventType());
		VIO_DEBUG("Event Category Flags: {0:d}; bin: {0:b}", e.getCategoryFlags());
		VIO_DEBUG(e.isInCategory(EventCategory::Application));
		VIO_DEBUG(e.isInCategory(EventCategory::Input));
		VIO_DEBUG(e.isInCategory(EventCategory::Keyboard));
		VIO_DEBUG(e.isInCategory(EventCategory::Mouse));
		VIO_DEBUG(e.isInCategory(EventCategory::MouseButton));
		VIO_DEBUG(e.isInCategory(EventCategory::None));

		EventDispatcher dis(e);
		dis.dispatch<MouseButtonPressedEvent>(eventHandling);
		VIO_TRACE("Is Event Handled: {}", e.getEventHandleStatus());
		while (true);
	}

	
}