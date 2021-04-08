#pragma once
#include "Camera.h"
#include "Violet/Core/DeltaTime.h"
#include "Violet/Events/Event.h"
#include "Violet/Events/MouseEvent.h"
#include <glm/glm.hpp>

namespace Violet {

	class VIOLET_API EditorCamera : public Camera {
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void setViewPortSize(float width, float height);

		void onUpdate(DeltaTime deltaTime);
		void onEvent(Event& event);

		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
		glm::mat4 getViewProjection() const { return m_projectionMatrix * m_viewMatrix; }

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3& getPosition() const { return m_position; }
		glm::quat getOrientation() const;

		float getPitch() const { return m_pitch; }
		float getYaw() const { return m_yaw; }
	private:
		void calculateProjectionMatrix();
		void calculateViewMatrix();

		bool onMouseScrolled(MouseScrolledEvent& event);

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec3 calculatePosition() const;

		glm::vec2 getPanSpeed() const;
		float getRotationSpeed() const;
		float getZoomSpeed() const;

	private:
		float m_FOV = 45.0f, m_aspectRatio = 0.0f, m_nearClip = 0.1f, m_farClip = 1000.0f;

		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalPoint = { 0.0f, 0.0f, 0.0f };  //Where the camera is focused

		glm::vec2 m_initialMousePosition = { 0.0f, 0.0f };

		float m_distanceZ = 10.0f;   //The look back distance from the focalPoint on the Z-Axis

		float m_pitch = 0.0f, m_yaw = 0.0f;

		float m_viewportWidth = 0.0f, m_viewportHeight = 0.0f;
	};
}