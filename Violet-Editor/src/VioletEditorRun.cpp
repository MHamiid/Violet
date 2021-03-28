#include <Violet.h>
#include <Violet/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Violet {

	class VioletEditor : public Application {

	public:
		VioletEditor() : Application("Violet-Editor"){

			pushLayer(new EditorLayer());
		}

	};

	Application* createApplication() {

		return new VioletEditor();
	}

}