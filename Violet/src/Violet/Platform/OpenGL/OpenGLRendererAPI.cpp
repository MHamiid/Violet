#include "VIOPCH.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
namespace Violet {
	//NOTE: If in debug mode the code that calls the function would be excluded which results this function to not be compiled and excluded also
	static void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         VIO_CORE_CRITICAL("[OPENGL MESSAGE] {0}", message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       VIO_CORE_ERROR("[OPENGL MESSAGE] {0}", message); return;
		case GL_DEBUG_SEVERITY_LOW:          VIO_CORE_WARN("[OPENGL MESSAGE] {0}", message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: VIO_CORE_TRACE("[OPENGL MESSAGE] {0}", message); return;
		}

		VIO_CORE_ASSERT(false, "[OpenGL] Unknown severity level!");
	}


	void OpenGLRendererAPI::init()
	{

		//Enable OpenGL message callbacks to the defined function whenever Violet is in debug mode
#ifdef VIO_DEBUG_MODE
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif


		//Setup blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blending function

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);  //Enable line Anti-Aliasing
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
		//Bind the vertex array
		vertexArray->bind();

		//If set to 0 draw the entire IndexBuffer
		uint32_t indicesCount = numberOfIndicesToDraw == 0 ? vertexArray->getIndexBuffer()->getCount() : numberOfIndicesToDraw;
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		//Bind the vertex array
		vertexArray->bind();

		glDrawArrays(GL_LINES, 0, vertexCount);
	}
	void OpenGLRendererAPI::setLineWidth(float width)
	{
		glLineWidth(width);
	}
}	