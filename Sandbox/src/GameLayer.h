#pragma once
#include "Violet.h"

class GameLayer : public Violet::Layer {

public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Violet::DeltaTime& deltaTime) override;

	virtual void onEvent(Violet::Event& e) override;

	void onImGuiRender() override;
private:
	Violet::ShaderLibrary m_shaderLibrary;  //Temp, Should be created in the renderer
	Violet::Ref<Violet::Shader> m_shader;
	Violet::Ref<Violet::Texture2D> m_texture, m_transparentTexture;
	Violet::Ref<Violet::VertexArray> m_vertexArray;

	Violet::OrthographicCameraController m_cameraController;

	glm::vec3 m_objectPosition;
	float m_objectMovementSpeed = 0.5f;
	glm::vec4 m_objectColor = { 1.0f, 0.7f, 0.6f, 1.0f };
};
