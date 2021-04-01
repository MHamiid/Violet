#pragma once
#include "Violet/Renderer/Camera.h"

namespace Violet {

	class VIOLET_API SceneCamera : public Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		
		void setViewPortSize(uint32_t width, uint32_t height);

		void setOrthographic(float size, float nearClip, float farClip);
		void setOrthographicSize(float size) { if (m_orthographicSize == size) return; m_orthographicSize = size; calculateProjectionMatrix(); }
		float getOrthographicSize() const { return m_orthographicSize; }
		void setOrthographicNearClip(float nearClip) { if (m_orthographicNear == nearClip) return; m_orthographicNear = nearClip; calculateProjectionMatrix(); }
		float getOrthographicNearClip() const { return m_orthographicNear; }
		void setOrthographicFarClip(float farClip) { if (m_orthographicFar == farClip) return; m_orthographicFar = farClip; calculateProjectionMatrix(); }
		float getOrthographicFarClip() const { return m_orthographicFar; }

	private:
		void calculateProjectionMatrix();
	private:
		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;
		float m_aspectRatio = 0.0f;
	};


}