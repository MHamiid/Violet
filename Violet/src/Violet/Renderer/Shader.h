#pragma once

namespace Violet {

	/*
	Interface
	*/
	class VIOLET_API Shader {
	public:
		virtual ~Shader() = default;
		virtual void bind() = 0;
		virtual void unBind() = 0;


	};

}