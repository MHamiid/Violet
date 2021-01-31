#include "VIOPCH.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_projectionMatrix( glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f), m_position(0.0f, 0.0f, 0.0f)
	{
		//Set the View Projection Matrix 
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::calculateViewMatrix()
	{
		//Set the new position and the rotation
		m_viewMatrix = glm::translate(glm::mat4(1.0f), -m_position) *
										glm::rotate(glm::mat4(1.0f), glm::radians(-m_rotationZ), glm::vec3(0, 0, 1));

		//Update the View Projection Matrix 
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}