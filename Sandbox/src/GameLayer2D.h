#pragma once
#include <Violet.h>

class GameLayer2D : public Violet::Layer {

public:
	GameLayer2D();
	virtual ~GameLayer2D() = default;
	void onAttach() override;
	void onDetach() override;
	void onUpdate(Violet::DeltaTime& deltaTime) override;
	void onImGuiRender() override;
	void onEvent(Violet::Event& e) override;
private:
	Violet::OrthographicCameraController m_cameraController;
	glm::vec3 m_objectPosition{ 0.0f, 0.0f, 0.0f };
	glm::vec4 m_objectColor = { 1.0f, 0.7f, 0.6f, 1.0f };

};