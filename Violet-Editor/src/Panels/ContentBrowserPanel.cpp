#include "ContentBrowserPanel.h"
#include <imgui.h>

namespace Violet {

	//TODO: Note that the assets path is hardcoded, extract the assets path from the Project (Project class not created yet).
	//g_assetsPath definition
	extern const std::filesystem::path g_assetsPath = "assets"; // [HARD-CODED][TEMP][GLOBAL]

	ContentBrowserPanel::ContentBrowserPanel() : m_currentDirectory(g_assetsPath)
	{
		/*Note: these textures is loaded everytime ContentBrowserPanel is constructed (@ex: everytime a new project is created/loaded)
		, optimally load the textures once in the EditorLayer for example and pass them here.*/

		m_directoryIcon = Texture2D::Create("resources/icons/ContentBrowser/directory.png");
		m_fileIcon = Texture2D::Create("resources/icons/ContentBrowser/file.png");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		/*Content Browser Panel*/
		ImGui::Begin("Content Browser");


		/*Render A Back Button*/
		//Note: The back button is not rendered if the current directory is the root directory (prevent access to files/folders outside the assets directory).
		if (m_currentDirectory != g_assetsPath) {

			if (ImGui::Button("<-")) {
				m_currentDirectory = m_currentDirectory.parent_path();  //Go back to the previous directory.
			}
		}

		/*Calculating how many columns are needed to display the items in the available region with a certain cell size for each item*/
		/*TODO: Add options for the user to adjust the padding + thumbnailSize*/
		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float contentBrowserPanelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = static_cast<int>(contentBrowserPanelWidth / cellSize);

		//Prevent crashing if the content browser panel has been resized to the minimum
		if (columnCount < 1) columnCount = 1;

		if (ImGui::BeginTable("Content Browser Table", columnCount, ImGuiTableFlags_SizingStretchSame))
		{
			/*TODO: Maybe store the directory entries in a list to avoid hitting the file system every single frame, render that list, and update the list if the directory has been modified (file/folder added/deleted)
			or update the list every specific interval (@ex: every second). */
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {

				const std::filesystem::path& path = directoryEntry.path();

				std::filesystem::path relativePath = std::filesystem::relative(path, g_assetsPath);   //Extract the current folder/file relative path to the s_assetsPath (removing s_assetsPath from the path)

				//Note: Currently works with english paths, otherwise it'd crash (we're reading the directory path as std::string).
				//TODO: Fix the previous note.
				std::string itemNameString = relativePath.filename().string();


				Ref<Texture2D> icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
				uint64_t textureID = icon->getTextureID();  //Change uint32_t to uint64_t to match with the 64 bit void pointer ( ImTextureID = void* ) when casting

				ImGui::TableNextColumn();
				//Remove the background color for the ImageButton
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushID(path.string().c_str()); //Push a unique ID for each ImageButton Item
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureID)
					, { thumbnailSize, thumbnailSize }
				, { 0, 1 }, { 1, 0 }); //Set the texture and flip it to it's original form, ImGui (0, 0) coordinates at top-left by default
				ImGui::PopStyleColor();
				ImGui::PopID();

				/*Drag/Drop Items*/
				if (ImGui::BeginDragDropSource())
				{
					const std::string itemRelativePath = relativePath.string();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemRelativePath.c_str(), itemRelativePath.size() + 1);  //NOTE: Add +1 to the size for the null termination character

					ImGui::EndDragDropSource();
				}

				//Double-click
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) /*Using this if condition instead of using if(ImGui::ImageButton) directly cause of ImGui::ImageButton uses the same textureID
																							  which fails to check for the condition for more that one item (they share the same textureID)*/
				{
					if (directoryEntry.is_directory()) {
						/*Directory*/
						m_currentDirectory /= directoryEntry.path().filename();  //Note: operator/ is overloaded so that it concatenates two paths with a directory separator 
					}
					else {
						/*File*/
					}
					//TODO: create a condition for a single click, which should probably show info in the status bar (to be done) about the clicked item (@ex: item file path, etc)
				}
				ImGui::TextWrapped(itemNameString.c_str());
			}
			ImGui::EndTable();
		}

		//TODO: Status bar (@ex: Tells the entire file path when a certain file is clicked)

		ImGui::End();
	}

}