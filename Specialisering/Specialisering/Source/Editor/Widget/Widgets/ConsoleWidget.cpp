#include "ConsoleWidget.h"
#include <string>
#include <imgui/imgui.h>
#include <Tools/ConsoleSystem.h>
bool Tga::ConsoleWidget::Update(EditorContext& /*aContext*/)
{
	std::string windName = "Console ##" + std::to_string(myID);
	if (ImGui::Begin(windName.c_str()))
	{

		if (ImGui::Button("Clear Console"))
		{
			Tga::Console::ClearLogs();
		}

		// Don't touch my (c)Logs				//https://www.youtube.com/watch?v=dXqtrHJAqVM
		auto logs = Tga::Console::GetLogs();
		for (int i = 0; i < logs.size(); i++)
		{
			std::string text = "Log [" + logs[i].time + "]: ";
			ImGui::Text(text.c_str());
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, { logs[i].color.myR , logs[i].color.myG, logs[i].color.myB, logs[i].color.myA });
			ImGui::Text(logs[i].message.c_str());
			ImGui::PopStyleColor();
		}
	}
	ImGui::End();
	return myIsOpen;
}
