#pragma once
#include "Core.h"
namespace Violet {
	class VIOLET_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};


	//To be defined in client.
	Application* createApplication();
}
