#include "VIOPCH.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
namespace Violet {
	void OpenGLRendererAPI::init()
	{
		//Setup blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blending function

		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::drawIndices(const Ref<VertexArray>& vertexArray, uint32_t numberOfIndicesToDraw)
	{
		//If set to 0 draw the entire IndexBuffer
		uint32_t indicesCount = numberOfIndicesToDraw == 0 ? vertexArray->getIndexBuffer()->getCount() : numberOfIndicesToDraw;
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	}
}	