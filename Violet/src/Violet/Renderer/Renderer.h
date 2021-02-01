#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Violet {
	

	class VIOLET_API Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO
		static void EndScene(); //TODO
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transformation = glm::mat4(1.0f)); //Temp
		static GraphicsAPI::API GetCurrentGraphicsAPI() { return GraphicsAPI::getAPI(); }
	private:
		struct SceneData{
			glm::mat4 viewProjectionMatrix;
		};
		static SceneData* s_SceneData;	
	};

}