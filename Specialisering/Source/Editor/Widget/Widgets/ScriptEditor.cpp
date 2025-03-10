#include "ScriptEditor.h"
// STD
#include <string>
#include <fstream>
#include <filesystem>
//Engine
#include <tge/io/Input.h>
// External
#include <imgui/imgui.h>
#include <imnodes/imnodes.h>
#include <imnodes/imnodes_internal.h>
#include <imgui/TextEditor.h>
// Game
#include <Scripting/ScriptJson.h>
#include <Scripting/Script.h>
#include <Scripting/ScriptManager.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptStringRegistry.h>
//Editor
#include <Tools/Commands/CommandManager.h>
#include "NodeScripting/SetOverridenValueCommand.h"	
#include "NodeScripting/MoveNodesCommand.h"	
#include "NodeScripting/CreateLinkCommand.h"	
#include "NodeScripting/CreateNodeCommand.h"	
#include <Tools/ImGuiFunctions.h>
//Game
#include <Scripting/Nodes/CommonNodes.h>
#include <Scripting/Nodes/DebugNodes.h>
#include <Scripting/Nodes/ShaderOutputNodes.h>
#include <Scripting/Nodes/BufferNodes.h>
#include <Scripting/Nodes/MathNodes.h>
#include <Tools/ConsoleSystem.h>
#include <Core/MainSingleton.h>
#include <Shader/ShaderManager.h>

#undef max

using namespace Tga;

static const char* locScriptPath = "/Scripts/";
Tga::ScriptNodeTypeId ShowNodeTypeSelectorForCategory(const Tga::ScriptNodeTypeRegistry::CategoryInfo& aCategory);

std::map<std::string_view, Tga::ScriptEditor::EditorScriptData> myOpenScripts;
std::string_view globalLastScript;

void mini_map_node_hovering_callback(int node_id, void* user_data)
{
	node_id;
	user_data;
	//Tga::ScriptNodeBase* node = func(node_id);
	//node;

	//std::string name = typeid(*node).name();
	//name.erase(name.begin(), name.begin() + 6);

	//ImGui::SetTooltip("%s", name.c_str());

	ImGui::SetTooltip("%s %i", "Node ", node_id);
}

Tga::ScriptEditor::ScriptEditor(int aUniqueID) : Widget(aUniqueID)
{
	myTextEditor = new TextEditor();
	myNodeFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("../EngineAssets/Font/NotoSans-Medium.ttf", 16);

	if (!myOpenScripts.empty())
	{
		myActiveScript = globalLastScript;
		return;
	}


	// To do, send this to a global bank instead of loading with every creation

	// init script
	std::string path = std::filesystem::current_path().generic_string() + "/Scripts/";
	std::filesystem::create_directory(path);

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() != ".json")
			continue;

		std::filesystem::path name = entry.path().stem();
		std::string nameString = name.generic_u8string();
		Script* script = ScriptManager::GetEditableScript(nameString);

		if (!script)
			continue;

		ScriptStringId nameStringId = ScriptStringRegistry::RegisterOrGetString(nameString.c_str());
		std::string_view nameStringView = ScriptStringRegistry::GetStringFromStringId(nameStringId);

		EditorScriptData data{ script, {}, ImNodes::EditorContextCreate() };
		data.nodeEditorContext = ImNodes::EditorContextCreate();
		data.latestSavedSequenceNumber = script->GetSequenceNumber();

		myOpenScripts.insert({ nameStringView, data });


		if (myActiveScript.empty())
			myActiveScript = nameStringView;

		globalLastScript = nameStringView;
	}

}

Tga::ScriptEditor::~ScriptEditor()
{
	delete myTextEditor;
	ClearCopiedSelection();
}

bool Tga::ScriptEditor::Update(EditorContext& aContext)
{
	if (!myIsOpen) return false;

	aContext;
	std::string lable = "ScriptEditor##" + std::to_string(myID);
	if (ImGui::Begin(lable.c_str(), &myIsOpen))
	{
		//ImGui::Text("ScriptEditor");

		RenderTopRow(aContext);
		RenderEditor(aContext);
		RenderPopups(aContext);
	}
	ImGui::End();

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (ImGui::IsKeyReleased(ImGuiKey_C))
		{
			int selectedNodeCount = ImNodes::NumSelectedNodes();
			std::vector<int> selectedNodes(selectedNodeCount);

			if (selectedNodeCount > 0)
			{
				ClearCopiedSelection();
				ImNodes::GetSelectedNodes(selectedNodes.data());

				for (int i = 0; i < selectedNodeCount; i++)
				{
					ScriptNodeId id; 
					id.id = selectedNodes[i];
					ScriptNodeBase& node = myOpenScripts[myActiveScript].script->GetNode(id);

					std::cout << typeid(node).name() << '\n';

					myCopiedSelection.push_back(node.Clone());
				}

			}

		}
		if (ImGui::IsKeyReleased(ImGuiKey_V))
		{
			// paste nodes

			//for (int i = 0; i < myCopiedSelection.size(); i++)
			//{

			//	/*Tga::CommandManager::DoCommand(
			//		std::make_shared<CreateNodeCommand>(
			//			&myOpenScripts[myActiveScript],
			//			&myOpenScripts[myActiveScript].selection,
			//			Tga::ScriptNodeTypeRegistry::GetTypeId(typeid(myCopiedSelection[i]).hash_code()),
			//			{0,0})
			//		);*/
			//}
		}
	}
	return myIsOpen;
}

void Tga::ScriptEditor::RenderTopRow(EditorContext& /*aContext*/)
{
	if (myActiveScript.empty())
	{
		myCreateNewCalled = ImGui::Button("Create new script...");
	}
	else
	{
		EditorScriptData& activeScriptData = myOpenScripts[myActiveScript];
		char nameText[128];
		if (activeScriptData.latestSavedSequenceNumber == activeScriptData.script->GetSequenceNumber())
		{
			sprintf_s(nameText, "%s", myActiveScript.data());
		}
		else
		{
			sprintf_s(nameText, "%s*", myActiveScript.data());
		}

		//Display Script Combo
		ImGui::SetNextItemWidth(200);
		if (ImGui::BeginCombo("##", nameText))
		{
			for (const auto& pair : myOpenScripts)
			{
				bool isSelected = pair.first == myActiveScript;


				if (pair.second.latestSavedSequenceNumber == pair.second.script->GetSequenceNumber())
					sprintf_s(nameText, "%s", pair.first.data());
				else
					sprintf_s(nameText, "%s*", pair.first.data());

				if (ImGui::Selectable(nameText, isSelected))
				{
					myActiveScript = pair.first;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			myCreateNewCalled = ImGui::Button("Create new script...");

			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		if (ImGui::DragFloat("Zoom", &mySizeMod, 0.005f, 0.1f, 10.f))
		{
			myNodeFont->Scale = mySizeMod;
		}

	}

	// Open and Create Scripts
	{
		if (myCreateNewCalled)
		{
			// Reset script name
			strncpy_s(myNewScriptName, "untitled", sizeof(myNewScriptName));
			myNewScriptName[sizeof(myNewScriptName) - 1] = '\0';
			ImGui::OpenPopup("Create new script");
			myCreateNewCalled = false;
			myNewShaderIsScript = false;
		}
		if (ImGui::BeginPopupModal("Create new script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("##", myNewScriptName, IM_ARRAYSIZE(myNewScriptName), ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Checkbox("IsShaderScript", &myNewShaderIsScript);
			ImGui::Separator();

			if (ImGui::Button("Create", ImVec2(120, 0)))
			{
				ScriptStringId scriptNameId = ScriptStringRegistry::RegisterOrGetString(myNewScriptName);
				std::string_view scriptName = ScriptStringRegistry::GetStringFromStringId(scriptNameId);

				if (myOpenScripts.find(scriptName) != myOpenScripts.end())
				{
					// just switch to script if one already exists with this filename
					// TODO: should probably present an error message/warning
					myActiveScript = scriptName;
				}
				else
				{
					ScriptManager::AddEditableScript(scriptName, std::make_unique<Script>());

					myOpenScripts.insert({ scriptName, EditorScriptData{ScriptManager::GetEditableScript(scriptName), {}, ImNodes::EditorContextCreate()} });
					myActiveScript = scriptName;
				}

				myOpenScripts[myActiveScript].script->SetIsShaderScript(myNewShaderIsScript);

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}
	}

	// 
	if (!myActiveScript.empty())
	{
		EditorScriptData& activeScript = myOpenScripts[myActiveScript];
		ImNodes::EditorContextSet(activeScript.nodeEditorContext);
		Script& script = *activeScript.script;

		if (ImGui::Button("Save"))
		{
			std::string basePath = std::filesystem::current_path().generic_string() + locScriptPath;
			std::filesystem::path scriptPath = std::filesystem::path(basePath) / std::filesystem::path(myActiveScript).replace_extension(".json");

			ScriptJson jsonData;
			script.WriteToJson(jsonData);

			std::ofstream out(scriptPath);
			out << jsonData.json;
			out.close();

			activeScript.latestSavedSequenceNumber = script.GetSequenceNumber();

			std::string msg = "Saved script : ";
			msg += myActiveScript.data();
			Tga::Console::LogMessage(msg, Tga::Console::Colors::Green);

			MSI_().GetShaderManager()->Recompile();
		}

		ImGui::SameLine();

		if (myOpenScripts[myActiveScript].script->IsShaderScript() && ImGui::Button("Export"))
		{
			ImGui::OpenPopup("ExportPopup");
		}

		ImGui::SameLine();

		if (myOpenScripts[myActiveScript].script->IsShaderScript() && ImGui::Button("Inspect Compiled HLSL"))
		{
			ImGui::OpenPopup("HlslEditor");
			myTextEditor->SetText(Tga::MainSingleton::GetInstance().GetShaderManager()->GetHLSLCode(myActiveScript.data()));

		}

		ImGui::SameLine();

		if (ImGui::Button("Revert"))
		{
			std::string basePath = std::filesystem::current_path().generic_string() + locScriptPath;
			std::filesystem::path scriptPath = std::filesystem::path(basePath) / std::filesystem::path(myActiveScript).replace_extension(".json");

			// Todo: pop up a warning, this clears undo history.
			CommandManager::Clear();
			script.Clear();

			std::ifstream file(scriptPath);
			if (file.is_open())
			{
				ScriptJson data = { nlohmann::json::parse(file) };
				file.close();
				script.LoadFromJson(data);
			}

			activeScript.latestSavedSequenceNumber = 0;
		}

		ImGui::SameLine();

		// Undo & Redo
		{
			ImGui::BeginDisabled(!Tga::CommandManager::CanUndo());

			if (ImGui::Button("Undo"))
			{
				Tga::CommandManager::Undo();
			}

			ImGui::EndDisabled();

			ImGui::SameLine();

			ImGui::BeginDisabled(!Tga::CommandManager::CanRedo());

			if (ImGui::Button("Redo"))
			{
				Tga::CommandManager::Redo();
			}

			ImGui::EndDisabled();
		}
	}
}


void Tga::ScriptEditor::RenderEditor(EditorContext& aContext)
{
	if (myActiveScript.empty()) return;

	// todo: keyboard bindings
	EditorScriptData& activeScript = myOpenScripts[myActiveScript];
	ImNodes::EditorContextSet(activeScript.nodeEditorContext);
	Script& script = *activeScript.script;

	float sizeModReverse = 1 / mySizeMod;

	// Prepare
	{
		for (ScriptNodeId currentNodeId = script.GetFirstNodeId(); currentNodeId.id != ScriptNodeId::InvalidId; currentNodeId = script.GetNextNodeId(currentNodeId))
		{
			Vector2f pos = script.GetPosition(currentNodeId);
			ImNodes::SetNodeGridSpacePos(currentNodeId.id, { pos.x * mySizeMod, pos.y * mySizeMod });
		}
		// if a link is in progress, set default style color to match the link color
		// all regular links will have their colors overriden so this does not affect them
		if (activeScript.inProgressLinkPin.id != ScriptPinId::InvalidId)
		{
			// todo check pin node also
			// if it is wrong type, highlight link as red

			const ScriptPin& pin = script.GetPin(activeScript.inProgressLinkPin);
			int typeIndex = (int)pin.dataType;

			ImNodesStyle& style = ImNodes::GetStyle();
			style.Colors[ImNodesCol_Link] = IM_COL32(ScriptLinkColors[typeIndex][0], ScriptLinkColors[typeIndex][1], ScriptLinkColors[typeIndex][2], 255);
			style.Colors[ImNodesCol_LinkSelected] = IM_COL32(ScriptLinkSelectedColors[typeIndex][0], ScriptLinkSelectedColors[typeIndex][1], ScriptLinkSelectedColors[typeIndex][2], 255);
			style.Colors[ImNodesCol_LinkHovered] = IM_COL32(ScriptLinkHoverColors[typeIndex][0], ScriptLinkHoverColors[typeIndex][1], ScriptLinkHoverColors[typeIndex][2], 255);
		}

	}

	ImGui::PushID(myID);
	ImNodes::BeginNodeEditor();

	ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight, mini_map_node_hovering_callback);
	// Render Nodes
	ImGui::PushFont(myNodeFont);
	for (ScriptNodeId currentNodeId = script.GetFirstNodeId();
		currentNodeId.id != ScriptNodeId::InvalidId;
		currentNodeId = script.GetNextNodeId(currentNodeId))
	{
		bool isNodeHighlighted = ImNodes::IsNodeSelected(currentNodeId.id) || activeScript.hoveredNode == currentNodeId;

		auto* currentNode = GetNode(currentNodeId.id);
		//todo add custom colors on node registry

		ImNodes::BeginNode(currentNodeId.id);
		{
			// TitleBar
			{
				ImNodes::BeginNodeTitleBar();

				//Check for custom name
				if (!currentNode->myNodeCustomName.empty())
				{
					ImGui::TextUnformatted(currentNode->myNodeCustomName.data());
				}
				else
				{
					ScriptNodeTypeId dataTypeId = script.GetType(currentNodeId);
					std::string_view string = ScriptNodeTypeRegistry::GetNodeTypeShortName(dataTypeId);
					ImGui::TextUnformatted(string.data());
				}
				ImNodes::EndNodeTitleBar();

				if (ImGui::IsItemHovered()
					&& ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
				{
					ImGui::OpenPopup("EditNodePresets");
				}


				if (ImGui::BeginPopup("EditNodePresets"))
				{
					//Name
					std::string title = currentNode->myNodeCustomName;
					{
						if (title.empty())
						{
							ScriptNodeTypeId dataTypeId = script.GetType(currentNodeId);
							title = ScriptNodeTypeRegistry::GetNodeTypeShortName(dataTypeId).data();
						}
						if (TextInputFix("## Name", &title))
						{
							currentNode->myNodeCustomName = title;
						}
					}
					ImGui::EndPopup();
				}

			}
			GetNode(currentNodeId.id)->CustomUiAbove(mySizeMod);
			ImVec2 cursorPos = ImGui::GetCursorPos();

			GetNode(currentNodeId.id)->CustomUiOverlaped(mySizeMod);

			ImVec2 centeredPosMax = ImGui::GetCursorPos();
			ImGui::SetCursorPos(cursorPos);



			// Left Pins
			float widthLeft = 100.f * mySizeMod;
			size_t inCount = 0;
			const ScriptPinId* inPins = script.GetInputPins(currentNodeId, inCount);
			{

				if (inCount == 0)
				{
					widthLeft = 0.f;
				}

				for (int i = 0; i < inCount; i++)
				{
					ScriptPinId pinId = inPins[i];
					const ScriptPin& pin = script.GetPin(pinId);
					std::string_view string = ScriptStringRegistry::GetStringFromStringId(pin.name);
					const float labelWidth = ImGui::CalcTextSize(string.data()).x * mySizeMod;

					widthLeft = std::max(widthLeft, labelWidth);
				}
			}
			// Right Pins
			float widthRight = 0.f * mySizeMod;
			size_t outCount = 0;
			const ScriptPinId* outPins = script.GetOutputPins(currentNodeId, outCount);
			{
				for (int i = 0; i < outCount; i++)
				{
					ScriptPinId pinId = outPins[i];
					const ScriptPin& pin = script.GetPin(pinId);
					std::string_view string = ScriptStringRegistry::GetStringFromStringId(pin.name);
					const float labelWidth = ImGui::CalcTextSize(string.data()).x * mySizeMod;

					widthRight = std::max(widthRight, labelWidth);
				}
			}
			if (widthLeft > 0.f && widthRight > 0.f)
			{
				widthRight += 20.f;
			}
			// Pins Render
			{
				// In Pins
				for (int i = 0; i < inCount; i++)
				{
					ScriptPinId pinId = inPins[i];
					const ScriptPin& pin = script.GetPin(pinId);
					int typeIndex = (int)pin.dataType;

					// Color of pin
					if (isNodeHighlighted)
					{
						ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(ScriptLinkSelectedColors[typeIndex][0],
							ScriptLinkSelectedColors[typeIndex][1],
							ScriptLinkSelectedColors[typeIndex][2], 255));
					}
					else
					{
						ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(ScriptLinkColors[typeIndex][0],
							ScriptLinkColors[typeIndex][1],
							ScriptLinkColors[typeIndex][2], 255));
					}
					ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(ScriptLinkHoverColors[typeIndex][0],
						ScriptLinkHoverColors[typeIndex][1],
						ScriptLinkHoverColors[typeIndex][2], 255));

					ImNodes::BeginInputAttribute(pinId.id);
					std::string_view string = ScriptStringRegistry::GetStringFromStringId(pin.name);

					const float labelWidth = ImGui::CalcTextSize(string.data()).x;
					ImGui::TextUnformatted(string.data());

					size_t connectionCount;
					script.GetConnectedLinks(pinId, connectionCount);

					if (connectionCount == 0)
					{

						bool hasOverridenValue = !std::holds_alternative<std::monostate>(pin.overridenValue.data);
						ScriptLinkData pinCurrentValue = hasOverridenValue ? pin.overridenValue : pin.defaultValue;
						ImGui::PushItemWidth(std::max(20.f, widthLeft - labelWidth));

						switch (pin.dataType)
						{
						case ScriptLinkDataType::Bool:
						{
							bool value = std::get<bool>(pinCurrentValue.data);
							ImGui::SameLine();
							if (ImGui::Checkbox("##", &value) && value != std::get<bool>(pinCurrentValue.data))
							{
								CommandManager::DoCommand(std::make_shared<SetOverridenValueCommand>(script, activeScript.selection, pinId, ScriptLinkData{ value }));
							}

							break;
						}
						case ScriptLinkDataType::Int:
						{
							int value = std::get<int>(pinCurrentValue.data);
							ImGui::SameLine();
							if (ImGui::InputInt("##", &value, 0) && value != std::get<int>(pinCurrentValue.data))
							{
								CommandManager::DoCommand(std::make_shared<SetOverridenValueCommand>(script, activeScript.selection, pinId, ScriptLinkData{ value }));
							}
							break;
						}
						case ScriptLinkDataType::Float:
						{
							float value = std::get<float>(pinCurrentValue.data);
							ImGui::SameLine();
							if (ImGui::DragFloat("##", &value, 0.001f) && value != std::get<float>(pinCurrentValue.data))
							{
								CommandManager::DoCommand(std::make_shared<SetOverridenValueCommand>(script, activeScript.selection, pinId, ScriptLinkData{ value }));
							}
							break;
						}
						case ScriptLinkDataType::String:
						{
							static char stringBuffer[256];

							ScriptStringId value = std::get<ScriptStringId>(pinCurrentValue.data);

							strncpy_s(stringBuffer, ScriptStringRegistry::GetStringFromStringId(value).data(), sizeof(stringBuffer));
							stringBuffer[sizeof(stringBuffer) - 1] = '\0';

							ImGui::SameLine();
							if (ImGui::InputText("##", stringBuffer, IM_ARRAYSIZE(stringBuffer)))
							{
								ScriptStringId newValue = ScriptStringRegistry::RegisterOrGetString(stringBuffer);

								CommandManager::DoCommand(std::make_shared<SetOverridenValueCommand>(script, activeScript.selection, pinId, ScriptLinkData{ newValue }));
							}
							break;
						}
						}

						ImGui::PopItemWidth();
					}


					ImNodes::EndInputAttribute();

					ImNodes::PopColorStyle();
					ImNodes::PopColorStyle();
				}

				if (outCount != 0)
				{
					ImGui::SetCursorPos(cursorPos);
				}

				// Out Pins
				for (int i = 0; i < outCount; i++)
				{
					ScriptPinId pinId = outPins[i];
					const ScriptPin& pin = script.GetPin(pinId);
					int typeIndex = (int)pin.dataType;

					// Pin Color
					if (isNodeHighlighted)
					{
						ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(ScriptLinkSelectedColors[typeIndex][0], ScriptLinkSelectedColors[typeIndex][1], ScriptLinkSelectedColors[typeIndex][2], 255));
					}
					else
					{
						ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(ScriptLinkColors[typeIndex][0], ScriptLinkColors[typeIndex][1], ScriptLinkColors[typeIndex][2], 255));
					}
					ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(ScriptLinkHoverColors[typeIndex][0], ScriptLinkHoverColors[typeIndex][1], ScriptLinkHoverColors[typeIndex][2], 255));

					ImNodes::BeginOutputAttribute(pinId.id);
					std::string_view string = ScriptStringRegistry::GetStringFromStringId(pin.name);

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + widthLeft + widthRight - ImGui::CalcTextSize(string.data()).x);

					ImGui::TextUnformatted(string.data());
					ImNodes::EndInputAttribute();


					ImNodes::PopColorStyle();
					ImNodes::PopColorStyle();
				}
			}
			/*if (inCount == 0)
			{
				ImGui::NewLine();
			}*/
			// Custom UI

			if (centeredPosMax.y >= ImGui::GetCursorPosY())
			{
				ImGui::SetCursorPos(centeredPosMax);
			}
			GetNode(currentNodeId.id)->CustomUiBelow(mySizeMod);

		}
		ImNodes::EndNode();

	}
	ImGui::PopFont();
	// Render Links
	for (ScriptLinkId linkId = script.GetFirstLinkId();
		linkId.id != ScriptLinkId::InvalidId;
		linkId = script.GetNextLinkId(linkId))
	{
		const ScriptLink& link = script.GetLink(linkId);

		const ScriptPin& sourcePin = script.GetPin(link.sourcePinId);
		const ScriptPin& targetPin = script.GetPin(link.targetPinId);

		int typeIndex = (int)(sourcePin.dataType == targetPin.dataType ? sourcePin.dataType : ScriptLinkDataType::UNKNOWN);
		ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(ScriptLinkColors[typeIndex][0], ScriptLinkColors[typeIndex][1], ScriptLinkColors[typeIndex][2], 255));
		ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(ScriptLinkSelectedColors[typeIndex][0], ScriptLinkSelectedColors[typeIndex][1], ScriptLinkSelectedColors[typeIndex][2], 255));
		ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(ScriptLinkHoverColors[typeIndex][0], ScriptLinkHoverColors[typeIndex][1], ScriptLinkHoverColors[typeIndex][2], 255));

		ImNodes::Link(linkId.id, link.sourcePinId.id, link.targetPinId.id);

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
	}

	ImGuiShortCuts(aContext);

	ImNodes::EndNodeEditor();
	ImGui::PopID();

	// Start of Link
	{
		{
			int startedLinkPinId;
			if (ImNodes::IsLinkStarted(&startedLinkPinId))
			{
				activeScript.inProgressLinkPin = { (unsigned int)startedLinkPinId };
			}
			else
			{
				activeScript.inProgressLinkPin = { ScriptPinId::InvalidId };
			}
		}

		{
			int hoveredNodeId;
			if (ImNodes::IsNodeHovered(&hoveredNodeId))
			{
				activeScript.hoveredNode = { (unsigned int)hoveredNodeId };
			}
			else
			{
				activeScript.hoveredNode = { ScriptNodeId::InvalidId };
			}
		}
	}

	// Cleanup
	for (ScriptNodeId currentNodeId = script.GetFirstNodeId();
		currentNodeId.id != ScriptNodeId::InvalidId;
		currentNodeId = script.GetNextNodeId(currentNodeId))
	{
		Vector2f oldPos = script.GetPosition(currentNodeId);
		ImVec2 gridPos = ImNodes::GetNodeGridSpacePos(currentNodeId.id);
		Vector2f newPos(gridPos.x * sizeModReverse, gridPos.y * sizeModReverse);


		//if (newPos.x != oldPos.x || newPos.y != oldPos.y)
		if ((int)newPos.x != (int)oldPos.x || (int)newPos.y != (int)oldPos.y)
		{
			if (myInProgressMove == nullptr)
			{
				myInProgressMove = std::make_shared<MoveNodesCommand>(script, activeScript.selection);
				Tga::CommandManager::DoCommand(myInProgressMove);
			}

			script.SetPosition(currentNodeId, { newPos.x, newPos.y });
			myInProgressMove->SetPosition(currentNodeId, oldPos, { newPos.x, newPos.y });
		}
	}

	// Clear in progress move if dragging ends
	if (!ImGui::IsMouseDown(0) && myInProgressMove)
	{
		myInProgressMove = nullptr;
	}

	// Create Link
	int startId, endId;
	if (ImNodes::IsLinkCreated(&startId, &endId))
	{
		ScriptPinId sourcePinId = { (unsigned int)startId };
		ScriptPinId targetPinId = { (unsigned int)endId };

		const ScriptPin& sourcePin = script.GetPin(sourcePinId);
		const ScriptPin& targetPin = script.GetPin(targetPinId);

		if (sourcePin.dataType == targetPin.dataType && sourcePin.dataType != ScriptLinkDataType::UNKNOWN)
		{
			/*	{
					Tga::ScriptCreationContext context(script, sourcePin.node);
					myOpenScripts[myActiveScript].script->GetNode(sourcePin.node).OnLinkCreateToMe(context, sourcePinId);
					myOpenScripts[myActiveScript].script->GetNode(sourcePin.node).OnLinkCreateToTarget(context, targetPinId);
				}
				{
					Tga::ScriptCreationContext context(script, targetPin.node);
					myOpenScripts[myActiveScript].script->GetNode(targetPin.node).OnLinkCreateToMe(context, targetPinId);
					myOpenScripts[myActiveScript].script->GetNode(targetPin.node).OnLinkCreateToTarget(context, sourcePinId);
				}*/

			std::shared_ptr<CreateLinkCommand> command = std::make_shared<CreateLinkCommand>(script, activeScript.selection, ScriptLink{ sourcePinId,targetPinId });
			Tga::CommandManager::DoCommand(command);
		}
	}

	// Destroy Link
	int linkId;
	if (ImNodes::IsLinkDestroyed(&linkId))
	{
		std::shared_ptr<DestroyNodeAndLinksCommand> command = std::make_shared<DestroyNodeAndLinksCommand>(script, activeScript.selection);
		command->Add(ScriptLinkId{ (unsigned int)linkId });
		Tga::CommandManager::DoCommand(command);
	}

	//Delete links and nodes if non imgui is active
	if ((ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace))
		&& (!ImGui::IsAnyItemActive()))
	{
		int numSelectedLinks = ImNodes::NumSelectedLinks();
		int numSelectedNodes = ImNodes::NumSelectedNodes();

		if (numSelectedLinks > 0 || numSelectedNodes > 0)
		{
			std::shared_ptr<DestroyNodeAndLinksCommand> command = std::make_shared<DestroyNodeAndLinksCommand>(script, activeScript.selection);

			if (numSelectedLinks > 0)
			{
				std::vector<int> selectedLinks;
				selectedLinks.resize(static_cast<size_t>(numSelectedLinks));
				ImNodes::GetSelectedLinks(selectedLinks.data());
				for (int i = 0; i < selectedLinks.size(); i++)
				{
					command->Add(ScriptLinkId{ (unsigned int)selectedLinks[i] });
				}
			}

			if (numSelectedNodes > 0)
			{
				static std::vector<int> selectedNodes;
				selectedNodes.resize(static_cast<size_t>(numSelectedNodes));
				ImNodes::GetSelectedNodes(selectedNodes.data());
				for (int i = 0; i < selectedNodes.size(); i++)
				{
					command->Add(ScriptNodeId{ (unsigned int)selectedNodes[i] });
				}
			}

			ImNodes::ClearNodeSelection();
			ImNodes::ClearLinkSelection();
			Tga::CommandManager::DoCommand(command);
		}
	}

	// on right click show add node UI
	{
		bool showAddNodeUI = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows) && (ImGui::IsMouseClicked(1) || ImGui::IsKeyPressed(ImGuiKey_Space)); // right mouse button

		if (showAddNodeUI && !ImGui::IsPopupOpen("EditNodePresets"))
		{
			ImGui::OpenPopup("Node Type Selection");
		}

		if (ImGui::BeginPopup("Node Type Selection"))
		{

			ImVec2 mPos = ImGui::GetMousePosOnOpeningCurrentPopup();
			ImVec2 canvasOriginScreenSpace = GImNodes->CanvasOriginScreenSpace;
			ImVec2 pannign = ImNodes::EditorContextGet().Panning;

			const ImVec2 clickPos =
			{
				mPos.x - canvasOriginScreenSpace.x - pannign.x,
				mPos.y - canvasOriginScreenSpace.y - pannign.y
			};

			const ScriptNodeTypeRegistry::CategoryInfo& category = ScriptNodeTypeRegistry::GetRootCategory();
			ScriptNodeTypeId typeToCreate = ShowNodeTypeSelectorForCategory(category);

			if (typeToCreate.id != ScriptNodeTypeId::InvalidId)
			{
				std::shared_ptr<CreateNodeCommand> command = std::make_shared<CreateNodeCommand>(script, activeScript.selection, typeToCreate, Vector2f{ clickPos.x, clickPos.y });
				Tga::CommandManager::DoCommand(command);
			}

			ImGui::EndPopup();
		}
	}
}


void Tga::ScriptEditor::RenderPopups(EditorContext& /*aContext*/)
{
	if (ImGui::BeginPopup("ExportPopup"))
	{
		if (ImGui::Selectable("Export to Console"))
		{
			MainSingleton::GetInstance().GetShaderManager()->
				ExportHLSLCodeToConsole(myActiveScript.data());
		}
		if (ImGui::Selectable("Export to ClipBoard"))
		{
			MainSingleton::GetInstance().GetShaderManager()->
				ExportHLSLCodeToClipBoard(myActiveScript.data());
		}
		if (ImGui::Selectable("Export to HLSL File"))
		{
			MainSingleton::GetInstance().GetShaderManager()->
				ExportHLSLToFile(myActiveScript.data());
		}

		ImGui::EndPopup();
	}

	ImVec2 wndPos = ImGui::GetWindowPos();
	ImVec2 wndSize = ImGui::GetWindowSize();
	if (ImGui::BeginPopup("HlslEditor"))
	{
		ImGui::Text(myActiveScript.data());
		ImGui::SetWindowPos(wndPos);

		myTextEditor->Render(myActiveScript.data(), wndSize);

		ImGui::EndPopup();
	}
}


ScriptNodeTypeId ShowNodeTypeSelectorForCategory(const ScriptNodeTypeRegistry::CategoryInfo& aCategory)
{
	ScriptNodeTypeId result = { ScriptNodeTypeId::InvalidId };

	for (const ScriptNodeTypeRegistry::CategoryInfo& childCategory : aCategory.childCategories)
	{
		std::string_view name = ScriptStringRegistry::GetStringFromStringId(childCategory.name);

		if (ImGui::BeginMenu(name.data()))
		{
			ScriptNodeTypeId type = ShowNodeTypeSelectorForCategory(childCategory);
			if (type.id != ScriptNodeTypeId::InvalidId)
			{
				result = type;
			}

			ImGui::EndMenu();
		}
	}

	for (ScriptNodeTypeId type : aCategory.nodeTypes)
	{
		std::string_view name = ScriptNodeTypeRegistry::GetNodeTypeShortName(type);
		if (ImGui::MenuItem(name.data()))
		{
			result = type;
			std::string msg = "Created node ";
			msg += name;
			Tga::Console::LogMessage(msg, Tga::Console::Colors::White);

		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(Tga::ScriptNodeTypeRegistry::GetNodeTooltip(type).data());
		}
	}

	return result;
}
void Tga::ScriptEditor::ImGuiShortCuts(EditorContext& /*aContext*/)
{
	if (ImGui::GetIO().MouseWheel != 0 && ImNodes::IsEditorHovered())
	{
		mySizeMod += ImGui::GetIO().MouseWheel * 0.1f;
		mySizeMod = std::max<float>(mySizeMod, 0.1f);
		myNodeFont->Scale = mySizeMod;
	}
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (ImGui::IsKeyReleased(ImGuiKey_S))
		{
			EditorScriptData& activeScript = myOpenScripts[myActiveScript];
			ImNodes::EditorContextSet(activeScript.nodeEditorContext);
			Script& script = *activeScript.script;

			std::string basePath = std::filesystem::current_path().generic_string() + locScriptPath;
			std::filesystem::path scriptPath = std::filesystem::path(basePath) / std::filesystem::path(myActiveScript).replace_extension(".json");

			ScriptJson jsonData;
			script.WriteToJson(jsonData);

			std::ofstream out(scriptPath);
			out << jsonData.json;
			out.close();

			activeScript.latestSavedSequenceNumber = script.GetSequenceNumber();

			std::string msg = "Saved script : ";
			msg += myActiveScript.data();
			Tga::Console::LogMessage(msg, Tga::Console::Colors::Green);

			MSI_().GetShaderManager()->Recompile();
		}

	}
}

Tga::ScriptNodeBase* Tga::ScriptEditor::GetNode(int aID)
{
	Tga::ScriptNodeId id;
	id.id = aID;
	return &myOpenScripts[myActiveScript].script->GetNode(id);
}

void Tga::ScriptEditor::ClearCopiedSelection()
{
	if (myCopiedSelection.empty()) return;

	for (int i = 0; i < myCopiedSelection.size(); i++)
	{
		delete myCopiedSelection[i];
	}
}

