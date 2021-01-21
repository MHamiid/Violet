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

class GameLayer : public Violet::Layer {

public:
	GameLayer(const std::string layerDebugName = "Layer") : Layer(layerDebugName) {}
	
	void onUpdate() override {
	
		//VIO_INFO("{0} OnUpdate" , getName());
	}

	void onEvent(Violet::Event& event) override {
		VIO_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		event.setEventHandleStatus(true); //Means "First Layer" won't receive events 
	}

};

class SandBox : public Violet::Application {

public:
	SandBox() {
		pushLayer(new GameLayer("First Layer"));
		pushLayer(new GameLayer("Second Layer"));
	}

};

Violet::Application* Violet::createApplication() {
    
	
	
	return new SandBox();


}