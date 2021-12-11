#pragma once
#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Violet {
	/*
	Interface
	*/
	class VIOLET_API GraphicsAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL = 1
		};
	public:
		virtual ~GraphicsAPI() = default;
		virtual void init() = 0;
		virtual void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndices(const Ref<VertexArray>& vertexArray, uint32_t numberOfIndicesToDraw) = 0;
		virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void setLineWidth(float width) = 0;
	public:
		static API getAPI() { return s_GraphicsAPI; }
		static Scoped<GraphicsAPI> Create();
	private:
		static API s_GraphicsAPI;  //What type of API is set to use
	};

}