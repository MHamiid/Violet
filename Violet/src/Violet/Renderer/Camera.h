#pragma once
#include <glm/glm.hpp>

namespace Violet {

	class VIOLET_API Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_projectionMatrix(projection) {}
		const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
	private:
		glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	};


}