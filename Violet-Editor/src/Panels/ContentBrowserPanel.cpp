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

		/*Calculating how many columns are needed to display the items in the available region with a certain cell size for each item*/
		/*TODO: Add options for the user to adjust the padding + thumbnailSize*/
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float contentBrowserPanelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(contentBrowserPanelWidth / cellSize);

		//Prevent crashing if the content browser panel has been resized to the minimum
		if (columnCount < 1) columnCount = 1;

		if (ImGui::BeginTable("Content Browser Table", columnCount, ImGuiTableFlags_SizingStretchSame))
		{
			 /*TODO: Maybe store the directory entries in a list to avoid hitting the file system every single frame, render that list, and update the list if the directory has been modified ( file/folder added/deleted)
			 or update the list every specific interval (@ex: every second). */
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {

				const std::filesystem::path& path = directoryEntry.path();

				std::filesystem::path relativePath = std::filesystem::relative(path, m_currentDirectory);   //Extract the current folder/file name in the current directory without the whole path.

				//Note: Currently works with english paths, otherwise it'd crash (we're reading the directory path as std::string).
				//TODO: Fix the previous note.
				std::string fileNameString = relativePath.filename().string();


				ImGui::TableNextColumn();
				if (ImGui::Button(fileNameString.c_str(), { thumbnailSize, thumbnailSize })) 
				{
					if (directoryEntry.is_directory()) {
						/*Directory*/
						m_currentDirectory /= directoryEntry.path().filename();  //Note: operator/ is overloaded so that it concatenates two paths with a directory separator 
					}
					else {
						/*File*/
					}
				}
			}
			ImGui::EndTable();
		}

		//TODO: Status bar (@ex: Tells the entire file path when a certain file is clicked)

		ImGui::End();
	}

}