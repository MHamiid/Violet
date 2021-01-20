#include "VIOPCH.h"
#include "Application.h"
namespace Violet {
	/*bool eventHandling(MouseButtonPressedEvent& ev) {
		VIO_INFO("Event Handle Function Called");
		return true;
	}*/

	Application::Application()
	{
		
		m_window = std::unique_ptr<Window>(Window::Create(WindowProperties("Violet Engine", 1280, 720 , true , std::bind(&Application::onEvent, this, std::placeholders::_1))));
		
	}
	Application::~Application()
	{
		
	}
	void Application::onEvent(Event& event) {

		//VIO_DEBUG(event.getName());
		EventDispatcher dispatcher(event);	
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1)); //Dispatch all window close events to onWindowClose function
	}
	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		m_applicationRunning = false;
		return true; //Confirm that the evet has been handled 
	}
	void Application::run()
	{
		VIO_DEBUG(m_window->getWidth());
		VIO_DEBUG(m_window->getHeight());
		VIO_DEBUG(m_window->isVSyncEnabled());
		
	//	MouseButtonPressedEvent e(30);
	//	VIO_DEBUG(e.getName());
	//	VIO_TRACE("Is Event Handled: {}",e.getEventHandleStatus());
	///*	VIO_DEBUG(e.getWidth());
	//	VIO_DEBUG(e.getHeight());*/
	//	VIO_DEBUG((int)e.getEventType());
	//	VIO_DEBUG("Event Category Flags: {0:d}; bin: {0:b}", e.getCategoryFlags());
	//	VIO_DEBUG(e.isInCategory(EventCategory::Application));
	//	VIO_DEBUG(e.isInCategory(EventCategory::Input));
	//	VIO_DEBUG(e.isInCategory(EventCategory::Keyboard));
	//	VIO_DEBUG(e.isInCategory(EventCategory::Mouse));
	//	VIO_DEBUG(e.isInCategory(EventCategory::MouseButton));
	//	VIO_DEBUG(e.isInCategory(EventCategory::None));

	//	EventDispatcher dis(e);
	//	dis.dispatch<MouseButtonPressedEvent>(eventHandling);
	//	VIO_TRACE("Is Event Handled: {}", e.getEventHandleStatus());
		while (m_applicationRunning) {
			m_window->onUpdate();	
		}
	}

	
}