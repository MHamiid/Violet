#pragma once
#include <filesystem>

namespace Violet {

	class ContentBrowserPanel {

	public:
		//TODO: Create a constructor that takes Project as an argument (Project class not created yet).
		ContentBrowserPanel();

		void onImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;
	};

}