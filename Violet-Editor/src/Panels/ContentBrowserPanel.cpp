#include "ContentBrowserPanel.h"
#include <imgui.h>

namespace Violet {

	//TODO: Note that the directory path is hardcoded, extract the directory from the Project (Project class not created yet).
	static const std::filesystem::path s_assestsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : m_currentDirectory(s_assestsPath)
	{
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		/*Content Browser Panel*/
		ImGui::Begin("Content Browser");


		/*Render A Back Button*/
		//Note: The back button is not rendered if the current directory is the root directory (prevent access to files/folders outside the assets directory).
		if (m_currentDirectory != s_assestsPath) {
			
			if (ImGui::Button("<-")) {
				m_currentDirectory = m_currentDirectory.parent_path();  //Go back to the previous directory.
			}
		}


		 /*TODO: Maybe store the directory entries in a list to avoid hitting the file system every single frame, render that list, and update the list if the directory has been modified ( file/folder added/deleted)
		 or update the list every specific interval (@ex: every second). */
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {

			const std::filesystem::path& path = directoryEntry.path();

			std::filesystem::path relativePath = std::filesystem::relative(path, m_currentDirectory);   //Extract the current folder/file name in the current directory without the whole path.

			//Note: Currently works with english paths, otherwise it'd crash (we're reading the directory path as std::string).
			//TODO: Fix the previous note.
			std::string fileNameString = relativePath.filename().string();

			if (directoryEntry.is_directory()) {

				if (ImGui::Button(fileNameString.c_str())) {
					m_currentDirectory /= directoryEntry.path().filename();  //Note: operator/ is overloaded so that it concatenates two paths with a directory separator 
				}
			}
			else {
				ImGui::Button(fileNameString.c_str());  //Render the files as buttons that don't do anything, just for visual consistency with the folders' buttons.
			}
		}

		ImGui::End();
	}

}