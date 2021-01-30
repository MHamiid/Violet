#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Violet {
	

	class VIOLET_API Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO
		static void EndScene(); //TODO
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray); //Temp
		static GraphicsAPI::API GetCurrentGraphicsAPI() { return GraphicsAPI::getAPI(); }
	private:
		struct SceneData{
			glm::mat4 viewProjectionMatrix;
		};
		static SceneData* s_SceneData;	
	};

}