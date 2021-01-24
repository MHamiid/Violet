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
		//Basic Tests for inputs
		if (Violet::Input::IsKeyPressed(Violet::Key::A))
			VIO_TRACE("Key A is Pressed");
		if (Violet::Input::IsMouseButtonPressed(Violet::Mouse::BUTTON_1)) {
			auto [x, y] = Violet::Input::GetMousePosition();
			VIO_TRACE("Mouse Button 1 is Pressed at position {0}, {1}", x, y);
		}
		if (Violet::Input::IsMouseButtonPressed(Violet::Mouse::BUTTON_2)) {
			auto [x, y] = Violet::Input::GetMousePosition();
			VIO_TRACE("Mouse Button 2 is Pressed at position {0}, {1}", x, y);
		}
	}

	void onEvent(Violet::Event& event) override {
		//VIO_TRACE("{0} OnEvent: {1}" ,getName() ,event.getName());
		//event.setEventHandleStatus(true); 
		/*if (event.getEventType() == Violet::EventType::KeyPressed) {
			Violet::KeyPressedEvent& keyEvent = static_cast<Violet::KeyPressedEvent&>(event);
			VIO_TRACE("Key {0} is pressed", (char)keyEvent.getKeyCode());
		
		}*/
	}

};

class SandBox : public Violet::Application {

public:
	SandBox() {
		pushLayer(new GameLayer("First Layer"));
		//pushLayer(new GameLayer("Second Layer"));
	}

};

Violet::Application* Violet::createApplication() {
    
	
	
	return new SandBox();


}