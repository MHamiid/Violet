#include "VIOPCH.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Violet {

	SceneCamera::SceneCamera()
	{
		calculateProjectionMatrix(); //Create a projection matrix with SceneCamera initialized parameters
	}

	void SceneCamera::setViewPortSize(uint32_t width, uint32_t height)
	{
		VIO_CORE_ASSERT(width > 0 && height > 0, "Attempting To Set Width Or Height To Value Less Than 0!");
		m_aspectRatio = (float)width / (float)height;
		calculateProjectionMatrix();
	}

	void SceneCamera::setPerspectiveProjection(float verticalFOV, float nearClip, float farClip)
	{
		/*NOTE: Changes the projection type to Perspective*/
		m_projectionType = ProjectionType::Perspective; //Make sure the projection type is set correctly
		m_perspectiveFOV = verticalFOV;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;
		calculateProjectionMatrix();
	}

	void SceneCamera::setOrthographicProjection(float size, float nearClip, float farClip)
	{
		/*NOTE: Changes the projection type to Orthographic*/
		m_projectionType = ProjectionType::Orthographic; //Make sure the projection type is set correctly
		m_orthographicSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		calculateProjectionMatrix();
	}

	void SceneCamera::calculateProjectionMatrix()
	{
		/*Update The Projection Matrix*/

		if (m_projectionType == ProjectionType::Perspective) 
		{
			m_projectionMatrix = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
		}
		else if (m_projectionType == ProjectionType::Orthographic)
		{
			m_projectionMatrix = glm::ortho(-m_orthographicSize * m_aspectRatio * 0.5f, m_orthographicSize * m_aspectRatio * 0.5f
				, -m_orthographicSize * 0.5f, m_orthographicSize * 0.5f, m_orthographicNear, m_orthographicFar);
		}

	}

}