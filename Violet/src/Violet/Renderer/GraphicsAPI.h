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
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndices(const Ref<VertexArray>& vertexArray) = 0;
	public:
		static API getAPI() { return s_GraphicsAPI; }
	private:
		static API s_GraphicsAPI;  //What type of api is set to use
	};

}