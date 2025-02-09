#include "Hierarchy.h"
#include <imgui/imgui.h>
#include <string>
#include <Scene/Scene.h>
#include <Object/GameObject.h>
#include <Tools/Selection.h>
Tga::Hierarchy::Hierarchy(int aUniqueID) :
	Widget(aUniqueID)
{
}

Tga::Hierarchy::~Hierarchy()
{
}

bool Tga::Hierarchy::Update(EditorContext& aContext)
{
	if (!myIsOpen) return false;

	std::string lable = "Hierarchy##" + std::to_string(myID);
	if (ImGui::Begin(lable.c_str(), &myIsOpen))
	{
		auto& objects = aContext.currentScene->GetGameObjects();

		static char input[200];
		ImGui::InputText("Search", &input[0], 200);
		for (int i = 0; i < objects.size(); i++)
		{
			std::string title = objects[i]->GetName();

			if (title.find(input) == -1)
			{
				continue;
			}

			if (title.empty())
			{
				title = "GameObject_" + std::to_string(objects[i]->GetID());
			}
			ImGui::PushID(objects[i]->GetID());

			if (ImGui::Selectable(title.c_str(),  Selection::Contains(objects[i])/*, nullptr,*/))
			{
				if (ImGui::GetIO().KeyCtrl == false)
				{
					Selection::ClearSelection();
				}
				Selection::ToggleSelect(objects[i]);
			}

			ImGui::PopID();
		}
	}
	ImGui::End();
	return myIsOpen;
}
