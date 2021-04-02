#pragma once
#include "Violet/Renderer/Camera.h"

namespace Violet {

	class VIOLET_API SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		
		void setViewPortSize(uint32_t width, uint32_t height);

		void setPerspective(float verticalFOV, float nearClip, float farClip);
		void setOrthographic(float size, float nearClip, float farClip);

		void setPerspectiveVerticalFOV(float verticalFov) { if (m_perspectiveFOV == verticalFov) return; m_perspectiveFOV = verticalFov; calculateProjectionMatrix(); }
		float getPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
		void setPerspectiveNearClip(float nearClip) { if (m_perspectiveNear == nearClip) return; m_perspectiveNear = nearClip; calculateProjectionMatrix(); }
		float getPerspectiveNearClip() const { return m_perspectiveNear; }
		void setPerspectiveFarClip(float farClip) { if (m_perspectiveFar == farClip) return; m_perspectiveFar = farClip; calculateProjectionMatrix(); }
		float getPerspectiveFarClip() const { return m_perspectiveFar; }

		void setOrthographicSize(float size) { if (m_orthographicSize == size) return; m_orthographicSize = size; calculateProjectionMatrix(); }
		float getOrthographicSize() const { return m_orthographicSize; }
		void setOrthographicNearClip(float nearClip) { if (m_orthographicNear == nearClip) return; m_orthographicNear = nearClip; calculateProjectionMatrix(); }
		float getOrthographicNearClip() const { return m_orthographicNear; }
		void setOrthographicFarClip(float farClip) { if (m_orthographicFar == farClip) return; m_orthographicFar = farClip; calculateProjectionMatrix(); }
		float getOrthographicFarClip() const { return m_orthographicFar; }

		ProjectionType getProjectionType() const { return m_projectionType; }
		void setProjectionType(ProjectionType projectionType) { if (m_projectionType == projectionType) return; m_projectionType = projectionType; calculateProjectionMatrix(); }

	private:
		void calculateProjectionMatrix();
	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;  //TODO: Change it to be Perspective by default when 3D is fully supported

		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};


}