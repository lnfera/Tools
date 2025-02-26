#include "Editor.h"
#include <imgui/imgui.h>

#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/GraphicsStateStack.h>
#include <tge/drawers/LineDrawer.h>
#include <tge/drawers/ModelDrawer.h>
#include <tge/primitives/LinePrimitive.h>
#include <tge/graphics/DX11.h>
#include <tge/io/Input.h>

#include <Object/I_AllComponents.h>
#include <Scene/Scene.h>
#include <Object/GameObject.h>
#include <tge/model/ModelInstance.h>
#include <tge/model/ModelFactory.h>
#include <tge/texture/TextureManager.h>

#pragma region Commands
#include <Tools/Commands/AddCommand.h>
#include <Tools/Commands/RemoveCommand.h>
#pragma endregion

#pragma region Tools
#include <tge/util/StringCast.h>
#include "Tools/ImGuiFunctions.h"
#include "Tools/Selection.h"
#include "Tools/Commands/CommandManager.h"
#include "Tools/ConsoleSystem.h"
#pragma endregion

#pragma region Widgets
#include "Widget/Widgets/AssetBrowser.h"
#include "Widget/Widgets/Hierarchy.h"
#include "Widget/Widgets/Inspector.h"
#include "Widget/Widgets/Viewport.h"
#include "Widget/Widgets/ScriptEditor.h"	
#include "Widget/Widgets/ConsoleWidget.h"	
#pragma endregion

#pragma region SubSystems
#include "Tools/SharedSystem/AssetBank.h"
#pragma endregion

Tga::Editor::Editor()
{
}

Tga::Editor::~Editor()
{
	myAssetBank->ShutDown();
	myAssetBank = nullptr;

	delete myScene;
	myScene = nullptr;

	for (int i = 0; i < myWidgets.size(); i++)
	{
		delete myWidgets[i];
		myWidgets[i] = nullptr;
	}
}

void Tga::Editor::Init()
{

	myModel = new Tga::ModelInstance();
	myModel->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	myModel->SetTexture(0, 0, Tga::Engine::GetInstance()->GetTextureManager().GetTexture(L"Textures/T_Default_BC.dds"));

	// Scene
	{
		myScene = new Tga::Scene("empty");
		myScene->LoadFromJson("World_01");
	}
	// Sub-Systems
	{
		myAssetBank = &Tga::AssetBank::GetInstance();
	}
	// Widgets
	{
		myWidgets.push_back(new Tga::AssetBrowser(++myWidgetUniqueID));
		myWidgets.push_back(new Tga::Hierarchy(++myWidgetUniqueID));
		myWidgets.push_back(new Tga::Inspector(++myWidgetUniqueID));
		myWidgets.push_back(new Tga::ScriptEditor(++myWidgetUniqueID));
		myWidgets.push_back(new Tga::Viewport(++myWidgetUniqueID));
		myWidgets.push_back(new Tga::ConsoleWidget(++myWidgetUniqueID));
	}

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("../EngineAssets/Font/NotoSansMono-Medium.ttf", 14);
	io.FontDefault = font1;
}

void Tga::Editor::Update(float aDeltaTime)
{
	// [Editor UI]
	{
		DX11::BackBuffer->SetAsActiveTarget();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		// [Window Flags]
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		// [Render Editor Main Window]
		ImGui::Begin("Editor", 0, window_flags);
		{
			ImGui::PopStyleVar(3);
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		}

		//Create Editor Context
		EditorContext context;
		context.deltaTime = aDeltaTime;
		context.currentScene = myScene;
		context.input = &Tga::Engine::GetInstance()->GetInputManager();


		//RenderSceneToTargets();
		RenderMenuBar(context);
		UpdateShortcuts(context);
		RenderWidgets(context);

		ImGui::End();
	}


}

void Tga::Editor::RenderMenuBar(EditorContext& /*aContext*/)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::Button("Start"))
			{
				system("GameLauncher_Debug.exe");
			}


			std::string lable = "Save to " + myScene->GetFileName();
			if (ImGui::MenuItem(lable.data()))
			{
				myScene->SaveToJson();

				std::string msg = "Saved scene ";
				msg += myScene->GetFileName();
				Tga::Console::LogMessage(msg, Tga::Console::Colors::Green);
			}

			if (ImGui::BeginMenu("Save Scene"))
			{
				static char name[200];
				if (ImGui::InputText("SceneName", &name[0], 200))
				{

				}
				if (ImGui::Button("Save"))
				{
					std::wstring msg = L"Would you like to save to " + string_cast<std::wstring>(name) + L"?";
					if (MessageBox(nullptr, msg.c_str(), L"Load File", MB_YESNO) == IDYES)
					{
						myScene->SaveToJson(name);

						std::string msg2 = "Saved scene to";
						msg2 += myScene->GetFileName();
						Tga::Console::LogMessage(msg2, Tga::Console::Colors::Green);
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Load Scene"))
			{
				std::filesystem::path p = std::filesystem::current_path().generic_string();
				p /= "Json";

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.2f, 1));
				ImGui::Text("Current Dir: %s", p.generic_string().c_str());
				ImGui::PopStyleColor();

				if (!std::filesystem::exists(p))
				{
					std::string path = std::filesystem::current_path().generic_string() + "/Json/";
					std::filesystem::create_directory(path);
					std::cout << "Created Directory /Json in Bin\n";
				}
				for (auto& entry : std::filesystem::directory_iterator(p))
				{
					std::string extension = entry.path().extension().string();
					if (extension == ".json")
					{
						if (ImGui::MenuItem(entry.path().filename().string().c_str()))
						{
							std::string filename = entry.path().filename().string();
							auto itr = filename.find(".json");
							while (itr != std::string::npos)
							{
								// Remove the substring
								filename.erase(itr, filename.length());
								// Look for the next occurrence
								itr = filename.find(".json");
							}

							std::wstring msg = L"Would you like to load " + string_cast<std::wstring>(filename) + L"?";
							if (MessageBox(nullptr, msg.c_str(), L"Load File", MB_YESNO) == IDYES)
							{
								Tga::Selection::ClearSelection();
								myScene->LoadFromJson(filename);

								std::string msg2 = "Loaded scene : ";
								msg2 += myScene->GetFileName();
								Tga::Console::LogMessage(msg2, Tga::Console::Colors::Green);
							}
						}
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("ImGui"))
		{
			if (ImGui::BeginMenu("Add Widget"))
			{
				if (ImGui::MenuItem("AssetBrowser"))
				{
					myWidgets.push_back(new Tga::AssetBrowser(++myWidgetUniqueID));
				}
				else if (ImGui::MenuItem("Inspector"))
				{
					myWidgets.push_back(new Tga::Inspector(++myWidgetUniqueID));
				}
				else if (ImGui::MenuItem("Hierarchy"))
				{
					myWidgets.push_back(new Tga::Hierarchy(++myWidgetUniqueID));
				}
				else if (ImGui::MenuItem("ScriptEditor"))
				{
					myWidgets.push_back(new Tga::ScriptEditor(++myWidgetUniqueID));
				}
				else if (ImGui::MenuItem("Viewport"))
				{
					myWidgets.push_back(new Tga::Viewport(++myWidgetUniqueID));
				}



				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Add Empty"))
			{
				Tga::CommandManager::DoCommand(std::make_shared<Tga::AddCommand>(Tga::AddCommand(myScene, { new GameObject() })));
			}

			ImGui::EndMenu();
		}

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - 200.f);
		ImGui::Text("Scene: %s", myScene->GetFileName().c_str());

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - 50.f);
		ImGui::Text("FPS: %i", (int)ImGui::GetIO().Framerate);

		ImGui::EndMainMenuBar();
	}
}
void Tga::Editor::RenderWidgets(EditorContext& aContext)
{
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

	for (int i = 0; i < myWidgets.size(); i++)
	{
		ImGui::SetNextWindowClass(&window_class);

		if (!myWidgets[i]->Update(aContext))
		{
			delete myWidgets[i];
			myWidgets.erase(myWidgets.begin() + i);
		}
	}
}
#include <imnodes/imnodes.h>
void Tga::Editor::UpdateShortcuts(EditorContext& /*aContext*/)
{
	// Control Commands



	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		// Save Function
		if (ImGui::IsKeyReleased(ImGuiKey_S))
		{
			myScene->SaveToJson();
			std::string msg = "Saved scene ";
			msg += myScene->GetFileName();
			Tga::Console::LogMessage(msg, Tga::Console::Colors::Green);
		}

		// [Undo + Redo]
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Z))
			{
				Tga::CommandManager::Redo();
			}
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_Z))
		{
			Tga::CommandManager::Undo();
		}

	}

	if (ImGui::IsKeyDown(ImGuiKey_F5))
	{
		system("GameLauncher_Debug.exe");
	}

}
