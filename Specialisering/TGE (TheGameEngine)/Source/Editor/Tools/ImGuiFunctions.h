#pragma once
#include <imgui/imgui.h>
namespace Tga
{
	inline static bool MenuItemWToolTip(char* aLable, char* aTooltip)
	{
		if (ImGui::MenuItem(aLable))
		{
			return true;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(aTooltip);
			ImGui::EndTooltip();
		}

		return false;
	}

	inline static bool TextInputFix(std::string aLabel, std::string* aTextVar, size_t aBufSize = 128, ImGuiInputTextFlags aFlags = 0, ImGuiInputTextCallback aCallback = NULL)
	{
		std::string tempString;
		tempString.reserve(aBufSize);
		tempString = *aTextVar;
		char* testText = tempString.data();
		bool result = ImGui::InputText(aLabel.c_str(), testText, aBufSize, aFlags, aCallback);
		*aTextVar = testText;
		return result;
	}

}