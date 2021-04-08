#include "VIOPCH.h"
#include "EditorCamera.h"
#include "Violet/Core/Input.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Violet {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		calculateViewMatrix();
	}

	void EditorCamera::setViewPortSize(float width, float height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		m_aspectRatio = width / height;
	
		calculateProjectionMatrix();
	}

	void EditorCamera::onUpdate(DeltaTime deltaTime)
	{
		if (Input::IsKeyPressed(Key::LEFT_ALT))
		{
			const glm::vec2& mousePosition{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 deltaMousePosition = (mousePosition - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mousePosition;

			if (Input::IsMouseButtonPressed(Mouse::BUTTON_LEFT))
			{
				mousePan(deltaMousePosition);
				calculateViewMatrix();
			}
			else if (Input::IsMouseButtonPressed(Mouse::BUTTON_RIGHT))
			{
				mouseRotate(deltaMousePosition);
				calculateViewMatrix();
			}

		}

	}

	void EditorCamera::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(VIO_BIND_EVENT_FUNCTION(EditorCamera::onMouseScrolled));
	}

	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::calculateProjectionMatrix()
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::calculateViewMatrix()
	{
		m_position = calculatePosition();
		
		glm::quat orientation = getOrientation();
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
	}

	bool EditorCamera::onMouseScrolled(MouseScrolledEvent& event)
	{
		float delta = event.getYOffset() * 0.1f;
		mouseZoom(delta);
		calculateViewMatrix();
		return true;
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		glm::vec2 panSpeed = getPanSpeed();
	
		m_focalPoint += -getRightDirection() * delta.x * panSpeed.x * m_distanceZ;
		m_focalPoint +=  getUpDirection() * delta.y * panSpeed.y * m_distanceZ;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		//Check if the Y-Axis is flipped or not
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
	
		m_yaw += yawSign * delta.x * getRotationSpeed();
		m_pitch += delta.y * getRotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		m_distanceZ -= delta * getZoomSpeed();
		if (m_distanceZ < 1.0f)
		{
			m_focalPoint += getForwardDirection();
			m_distanceZ = 1.0f;
		}
	}

	glm::vec3 EditorCamera::calculatePosition() const
	{
		return m_focalPoint - getForwardDirection() * m_distanceZ;  //Backward of the focalPoint
	}

	glm::vec2 EditorCamera::getPanSpeed() const
	{
		/*The Smaller The ViewPort The Bigger The Speed*/
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;   // X^2 - 0.1178X + 0.3021 

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;   // X^2 - 0.1178X + 0.3021 

		return { xFactor, yFactor };
	}

	float EditorCamera::getRotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::getZoomSpeed() const
	{
		float distance = m_distanceZ * 0.2f;
		distance = std::max(distance, 0.0f);  // min =  0.0f
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100.0f
		return speed;
	}

}