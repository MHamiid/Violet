#pragma once

#ifdef VIO_PLATFORM_WINDOWS

Violet::Application* Violet::createApplication();

int main(int argc , char** argv){

	Violet::Application* app = Violet::createApplication();
	app->run();
	delete app;


}
#endif