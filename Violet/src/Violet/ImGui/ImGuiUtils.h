#pragma once

namespace Violet::Utils::ImGuiUtils {  //NOTE: The namespace is named ImGuiUtils rather than ImGui to avoid collision with the original ImGui namespace
	
	template<typename UIFunction>
	void DrawWithHiddenStyle(bool hide, UIFunction UIFUNC)
	{
		bool itemHidden = false;
		if (hide)
		{
			/*Draw With Hidden Style*/
			itemHidden = true;
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		UIFUNC(itemHidden);

		if (itemHidden)
		{
			/*Pop Drawing With Hidden Style*/
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}

}