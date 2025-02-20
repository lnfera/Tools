#include "Gizmo.h"

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include <tge/graphics/Camera.h>

#include "../Commands/CommandManager.h"
#include <Scene/Scene.h>

//#include <CommandManager/Commands/TransformCommand.h>
//#include <CommandManager/Commands/ChangeTextureCommand.h>
#include "../Selection.h"
#include "../Commands/TransformCommand.h"
#include <Scene/Scene.h>
#include <Object/GameObject.h>
//#include <SeneHierarchy/SceneHierarchy.h>

using namespace Tga;

static Vector3f CalcSelectionPosition() 
{
	auto selection = Selection::GetSelection();

	if (selection.size() == 1) {
		GameObject* object = Selection::GetSelection()[0];
		return object->GetTransform().GetPosition();
	}

	Vector3f position;

	for (GameObject* o : selection)
	{
		position += o->GetTransform().GetPosition();
	}

	position /= (float)selection.size();

	return position;
}

void Gizmos::DrawSettings()
{
	if (ImGui::Begin("Gizmo settings", 0)) {
		{ // Operation settings
			if (ImGui::RadioButton("Translate", myCurrentOperation == ImGuizmo::TRANSLATE)) {
				myCurrentOperation = ImGuizmo::TRANSLATE;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", myCurrentOperation == ImGuizmo::ROTATE)) {
				myCurrentOperation = ImGuizmo::ROTATE;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", myCurrentOperation == ImGuizmo::SCALE)) {
				myCurrentOperation = ImGuizmo::SCALE;
			}
		}
		{ // World/local -space mode
			if (myCurrentOperation != ImGuizmo::SCALE)
			{
				if (ImGui::RadioButton("Local", myCurrentMode == ImGuizmo::LOCAL))
					myCurrentMode = ImGuizmo::LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", myCurrentMode == ImGuizmo::WORLD))
					myCurrentMode = ImGuizmo::WORLD;
			}
		}
		{ // Settings for snapping
			switch (myCurrentOperation)
			{
			case ImGuizmo::TRANSLATE:
				ImGui::Text("Translation Snap");
				ImGui::DragFloat("##", &mySnap.pos);
				break;
			case ImGuizmo::ROTATE:
				ImGui::Text("Angle Snap");
				ImGui::DragFloat("##", &mySnap.rot);
				break;
			case ImGuizmo::SCALE:
				ImGui::Text("Scale Snap");
				ImGui::DragFloat("##", &mySnap.scale);
				break;
			}
		}
	}
	ImGui::End();

	{ // Keyboard shortcuts
		if (ImGui::IsAnyItemActive() == false && ImGui::GetIO().KeyCtrl == false) {
			if (ImGui::IsKeyPressed(ImGuiKey_W)) {
				myCurrentOperation = ImGuizmo::TRANSLATE;
			} if (ImGui::IsKeyPressed(ImGuiKey_E)) {
				myCurrentOperation = ImGuizmo::ROTATE;
			} if (ImGui::IsKeyPressed(ImGuiKey_R)) {
				myCurrentOperation = ImGuizmo::SCALE;
			}
		}
	}
}

void Gizmos::DrawGizmos(Camera& aCamera)
{
	if (Selection::GetSelection().size() <= 0) { return; }
	auto selection = Selection::GetSelection();

	ImGui::SetItemDefaultFocus();

	auto io = ImGui::GetIO();
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

		//ImGui::GetWindowPos
		float left = ImGui::GetWindowPos().x;
		float top = ImGui::GetWindowPos().y;
		float right = ImGui::GetWindowSize().x;
		float bottom = ImGui::GetWindowSize().y;

		ImGuizmo::SetRect(left, top, right, bottom);

		const Matrix4x4f& view = Matrix4x4f::GetFastInverse(aCamera.GetTransform());
		const Matrix4x4f& proj = aCamera.GetProjection();

		if (Selection::GetSelection().size() > 0 && ImGuizmo::IsUsing() == false) 
		{
			myGizmoTransform.SetPosition(CalcSelectionPosition());
		}

		Matrix4x4f lastTransform;
		{
			float snap[3];
			switch (myCurrentOperation)
			{
			case ImGuizmo::TRANSLATE: snap[0] = snap[1] = snap[2] = mySnap.pos;	break;
			case ImGuizmo::SCALE: snap[0] = snap[1] = snap[2] = mySnap.scale;	break;
			case ImGuizmo::ROTATE: snap[0] = snap[1] = snap[2] = mySnap.rot;	break;
			default: break;

			}

			if ((ImGuizmo::MODE)myCurrentMode == ImGuizmo::LOCAL)
			{
				myGizmoTransform  = Selection::GetSelection().back()->GetTransform();
			}

			lastTransform = myGizmoTransform * lastTransform;
				//ApplyTransform(myGizmoTransform);

			ImGuizmo::Manipulate(
				&view(1,1),
				&proj(1,1),
				(ImGuizmo::OPERATION)myCurrentOperation,
				(ImGuizmo::MODE)myCurrentMode,
				myGizmoTransform.GetDataPtr(),
				nullptr,
				snap
			);
		}

		if (!io.KeyAlt && myIsManipulating && io.MouseReleased[ImGuiMouseButton_Left]) 
		{

			myTransformCommand.EndCommand();

			myIsManipulating = false;
		}

		if (!io.KeyAlt && ImGuizmo::IsUsing()) 
{
			if (io.MouseClicked[ImGuiMouseButton_Left]) 
			{
				myIsManipulating = true;
				
				myTransformCommand.BeginCommand(selection);
			}

			for (GameObject* object : selection) 
			{
				Matrix4x4f localMatrix = object->GetTransform() * lastTransform.GetInverse() * myGizmoTransform;

				object->GetTransform() = localMatrix;

			}
		}

	}
}
