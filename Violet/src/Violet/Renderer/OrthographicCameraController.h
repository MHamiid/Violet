#pragma once
#include "Violet/Renderer/OrthographicCamera.h"
#include "Violet/Core/DeltaTime.h"
#include "Violet/Events/ApplicationEvent.h"
#include "Violet/Events/MouseEvent.h"

namespace Violet {


	class VIOLET_API OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);
		void onUpdate(DeltaTime deltaTime);
		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return m_camera; }
		const OrthographicCamera& getCamera() const { return m_camera; } //Provide a const verstion of the function
		float getAspectRatio() { return m_aspectRatio; }
		float getZoomLevel() { return m_zoomLevel; }
		/*
		*	Following functions calculated from the glm::ortho projection
		*	m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel , m_zoomLevel)
		*/
		float getLeft() { return (-m_aspectRatio * m_zoomLevel) + m_cameraPosition.x; }
		float getRight() { return (m_aspectRatio * m_zoomLevel) + m_cameraPosition.x; }
		float getBottom() { return -m_zoomLevel + m_cameraPosition.y; }
		float getTop() { return m_zoomLevel + m_cameraPosition.y; }
		float getWidth() { return m_aspectRatio * m_zoomLevel * 2.0f; }
		float getHeight() { return m_zoomLevel * 2.0f; }
	private:
		//Functions to be dispatched in onEvent
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		OrthographicCamera m_camera; //The camera which the class controls
		bool m_rotation = false;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraTranslationSpeed = 0.3f;
		float m_cameraRotation = 0.0f;
		float m_cameraRotationSpeed = 90.0f;
	};

}