#pragma once
#include <glm/glm.hpp>

namespace Violet {

	class VIOLET_API OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void setProjection(float left, float right, float bottom, float top);

		void setPosition(const glm::vec3& position) { if (m_position == position) return; m_position = position; calculateViewMatrix(); }
		const glm::vec3& getPosition() { return m_position; }

		void setRotationZ(float rotationZ) { if (m_rotationZ == rotationZ) return; m_rotationZ = rotationZ; calculateViewMatrix(); }
		float getRotationZ() { return m_rotationZ; }
		
		
		const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void calculateViewMatrix();
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;
		glm::vec3 m_position;
		float m_rotationZ = 0.0f;

	};

}