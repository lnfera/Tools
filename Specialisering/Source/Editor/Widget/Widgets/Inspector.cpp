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
			Tga::Matrix4x4f& transform = obj->GetTransform();

			bool didChangeTransform = false;

			Vector3f pos, scale;
			Quaternionf rot;
			transform.DecomposeMatrix(pos, rot, scale);

			if (myIsEditingMatrix == false)
			{
				myTransformCommand = {};
				myTransformCommand.BeginCommand({ obj });
				myRotationalEuler = rot.GetYawPitchRoll();;
			}

			//@todo add snapping with gizmo selection
			if (ImGui::DragFloat3("Position", &pos.x, 0.25f))
			{
			}
			if (ImGui::IsItemActive())
			{
				didChangeTransform = true;
			}
			if (ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.1f))
			{
			}
			if (ImGui::IsItemActive())
			{
				didChangeTransform = true;
			}
			if (ImGui::DragFloat3("Rotation", &myRotationalEuler.x, 0.1f))
			{
			}
			if (ImGui::IsItemActive())
			{
				didChangeTransform = true;
			}

			if (didChangeTransform)
			{
				Matrix4x4f translation = Matrix4x4f::CreateFromTranslation(pos);
				Matrix4x4f rotation = Matrix4x4f::CreateFromRotation(myRotationalEuler);
				Matrix4x4f scaling = Matrix4x4f::CreateFromScale(scale);

				transform = scaling * rotation * translation;


				myIsEditingMatrix = true;
				ImGui::Text("Changing Matrix...");
			}
			else
			{
				if (myIsEditingMatrix)
				{
					myTransformCommand.EndCommand();
					myIsEditingMatrix = false;
				}
			}

		}

		int componentItr = 0;
		for (auto* comp : obj->GetComponentList())
		{
			ImGui::PushID(componentItr++);

			std::string compName = typeid(*comp).name();
			compName.erase(compName.begin(), compName.begin() + 5);

			ImGui::SetNextItemOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
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
