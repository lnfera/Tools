#include "Inspector.h"
#include <string>
//#include <imgui/imgui.h>
#include <Tools/Selection.h>
#include <Object/GameObject.h>
#include <Tools/ImGuiFunctions.h>
#include <Object/I_AllComponents.h>
bool Tga::Inspector::Update(EditorContext&)
{
	if (!myIsOpen) return false;

	std::string lable = "Inspector##" + std::to_string(myID);
	if (ImGui::Begin(lable.c_str(), &myIsOpen))
	{
		std::vector<GameObject*> selection = Tga::Selection::GetSelection();

		if (selection.empty())
		{
			ImGui::End();
			return myIsOpen;
		}

		Tga::GameObject* obj = selection.back();

		//Name
		std::string title = obj->GetName();
		{
			if (title.empty())
			{
				title = "GameObject_" + std::to_string(obj->GetID());
			}
			if (TextInputFix("## Name", &title))
			{
				obj->SetName(title);
			}
		}

		//Transform Modification
		{
			Transform& transform = obj->GetTransform();
			Vector3f pos = transform.GetPosition();
			if (ImGui::DragFloat3("Position", &pos.x, 1.0f))
			{
				transform.SetPosition(pos);
			}
			Vector3f scale = transform.GetScale();
			if (ImGui::DragFloat3("Scale", &scale.x, 0.01f))
			{
				transform.SetScale(scale);
			}

			/*Vector3f rotationBefore = transform.GetRotationRad();
			Vector3f rotation = rotationBefore / CU::PI<float> *180.f;
			if (ImGui::DragFloat3("Rotation", &rotation.x, 0.5f))
			{
				rotation = rotation / 180.f * CU::PI<float>;
				Vector3f rotationDelta = rotationBefore.To(rotation);

				for (int i = 0; i < 3; i++)
				{
					transform.Rotate(CU::UnitVector<3, float>(i), rotationDelta[i]);
				}
			}*/
		}

		int componentItr = 0;
		for (auto* comp :  obj->GetComponentList())
		{
			ImGui::PushID(componentItr++);

			std::string compName = typeid(*comp).name();
			compName.erase(compName.begin(), compName.begin() + 5);

			if (ImGui::TreeNode(compName.c_str()))
			{
				comp->ImGuiAccess();
				ImGui::TreePop();
			}

			if (ImGui::BeginPopupContextItem("ComponentContextMenu"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					// Code to remove the component from the object
					obj->RemoveComponent(comp);
					// Exit the loop to avoid accessing an invalid iterator after removal
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiButtonFlags_MouseButtonRight))
			{
				ImGui::OpenPopup("Remove Component");

			}

			ImGui::PopID();
		}

		if (ImGui::BeginCombo("##AddComponent", "AddComponent"))
		{
			DisplayComponentAddList(obj);

			ImGui::EndCombo();
		}

	}
	ImGui::End();

    return myIsOpen;
}

void Tga::Inspector::DisplayComponentAddList(Tga::GameObject* aObject)
{

	if (ImGui::BeginMenu("Model"))
	{
		if (ImGui::MenuItem("RenderComponent"))
		{
			aObject->AddComponent<Tga::RenderComponent>();
		}
		ImGui::EndMenu();
	}
}
