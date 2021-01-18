#pragma once

#ifdef VIO_PLATFORM_WINDOWS

//Violet::Application* Violet::createApplication();

int main(int argc , char** argv){
	Violet::Log::init();
	

	VIO_CORE_DEBUG("DEBUG");
	VIO_CORE_TRACE("TRACE");
	VIO_CORE_INFO("INFO");
	VIO_CORE_WARN("WARN");
	VIO_CORE_ERROR("ERROR");
	VIO_CORE_CRITICAL("CRITICAL");

	VIO_DEBUG("DEBUG");
	VIO_TRACE("TRACE");
	VIO_INFO("INFO");
	VIO_WARN("WARN");
	VIO_ERROR("ERROR");
	VIO_CRITICAL("CRITICAL");

	Violet::Application* app = Violet::createApplication();
	app->run();
	delete app;


}
#endif