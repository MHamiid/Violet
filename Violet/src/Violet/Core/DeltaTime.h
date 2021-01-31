#pragma once

namespace Violet {

	class VIOLET_API DeltaTime {
	public:
		DeltaTime() = default;

		float getSeconds() const{return m_deltaTime; }
		float getMilliSeconds() const { return m_deltaTime * 1000.0f; }
	public:
		operator float() const { return m_deltaTime; } //To cast implictally to float in usage later on rather than using deltaTimeInstance.getSeconds();
	private:
		void calculateDeltaTime(float currentTime) { m_deltaTime = currentTime - m_lastFrameTime;  m_lastFrameTime = currentTime; }
	private:
		float m_deltaTime = 0.0f;
		float m_lastFrameTime = 0.0f;
		friend class Application;
	};

}