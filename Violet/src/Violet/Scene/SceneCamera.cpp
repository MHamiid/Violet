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
		m_aspectRatio = (float)width / (float)height;
		calculateProjectionMatrix();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		m_orthographicSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		calculateProjectionMatrix();
	}

	void SceneCamera::calculateProjectionMatrix()
	{
		//Update the projection matrix
		m_projectionMatrix = glm::ortho(-m_orthographicSize * m_aspectRatio * 0.5f, m_orthographicSize * m_aspectRatio * 0.5f
			, -m_orthographicSize * 0.5f, m_orthographicSize * 0.5f, m_orthographicNear, m_orthographicFar);
	}

}