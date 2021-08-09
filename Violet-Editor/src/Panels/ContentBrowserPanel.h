#pragma once
#include <filesystem>
/*TODO: Make it that we can include "Texture.h" without having to include "core.h" explicitly
("core.h" has to be included before "Texture.h" as that "core.h" contains the VIOLET_API macro)*/
#include "Violet/core/core.h"
#include "Violet/Renderer/Texture.h"

namespace Violet {

	class ContentBrowserPanel {

	public:
		//TODO: Create a constructor that takes Project as an argument (Project class not created yet).
		ContentBrowserPanel();

		void onImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;
		Ref<Texture2D> m_directoryIcon;
		Ref<Texture2D> m_fileIcon;
	};

}