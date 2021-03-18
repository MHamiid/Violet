#include "VIOPCH.h"
#include "OrthographicCameraController.h"
#include "Violet/Input.h"
#include "Violet/KeyCodes.h"

namespace Violet {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel , m_zoomLevel),m_aspectRatio(aspectRatio), m_rotation(rotation)
	{

	}

	void OrthographicCameraController::onUpdate(DeltaTime deltaTime)
	{
		//Camera controls

		if (Input::IsKeyPressed(Violet::Key::A)) {
			m_cameraPosition.x -= m_cameraTranslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(Violet::Key::D)) {
			m_cameraPosition.x += m_cameraTranslationSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(Violet::Key::W)) {
			m_cameraPosition.y += m_cameraTranslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(Violet::Key::S)) {
			m_cameraPosition.y -= m_cameraTranslationSpeed * deltaTime;
		}

		//For rotation
		if (m_rotation) {  //If rotation is enabled

			if (Input::IsKeyPressed(Violet::Key::E)) {
				m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
			}
			else if (Input::IsKeyPressed(Violet::Key::Q)) {
				m_cameraRotation += m_cameraRotationSpeed * deltaTime;
			}
			//Update the rotation value
			m_camera.setRotationZ(m_cameraRotation);
		}

		//Update the position value
		m_camera.setPosition(m_cameraPosition);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(VIO_BIND_EVENT_FUNCTION(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(VIO_BIND_EVENT_FUNCTION(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{	
		m_zoomLevel -= e.getYOffset() * 0.25f;
		//clip to a min and max values
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_zoomLevel = std::min(m_zoomLevel, 20.0f);

		m_cameraTranslationSpeed = m_zoomLevel;  //Speed increases as we zoom out and vice versa
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

}