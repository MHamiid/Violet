#include <Violet.h>
#include <Violet/Core/EntryPoint.h>
#include "GameLayer.h"
#include "GameLayer2D.h"
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

class SandBox : public Violet::Application {

public:
	SandBox() {

		//pushLayer(new GameLayer());
		pushLayer(new GameLayer2D());
	}

};

Violet::Application* Violet::createApplication() {
    
	
	
	return new SandBox();


}