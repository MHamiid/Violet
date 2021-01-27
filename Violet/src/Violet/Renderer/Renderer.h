#pragma once

namespace Violet {
	enum class RendererAPI {
		NONE = 0,
		OPENGL = 1
	};

	class VIOLET_API Renderer {
	public:
		static RendererAPI GetCurrentRendererApi() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;

	};

}