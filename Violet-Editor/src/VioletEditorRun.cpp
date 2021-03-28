#include <Violet.h>
#include <Violet/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Violet {

	class VioletEditor : public Application {

	public:
		VioletEditor() {

			//pushLayer(new GameLayer());
			pushLayer(new EditorLayer());
		}

	};

	Application* createApplication() {



		return new VioletEditor();


	}

}