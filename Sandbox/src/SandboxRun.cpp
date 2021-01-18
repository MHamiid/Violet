#include <Violet.h>


//Violet::Application* Violet::createApplication();
//
//int main(int argc, char** argv) {
//
//	Violet::Application* app = Violet::createApplication();
//	app->run();
//	delete app;
//
//
//}

Violet::Application* Violet::createApplication() {
    
   
   /* Log::GetClientLogger()->warn("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");
    Log::GetCoreLogger()->warn("Welcome to spdlog!");*/
	return new Violet::Application();


}