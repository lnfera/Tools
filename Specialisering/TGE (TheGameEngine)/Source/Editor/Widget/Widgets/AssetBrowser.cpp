#include "AssetBrowser.h"

#include <string>
#include <imgui/imgui.h>
#include "../../Tools/SharedSystem/AssetBank.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>

Tga::AssetBrowser::AssetBrowser(int aUniqueID)
	: Widget(aUniqueID)
{
	//myAssetBank = std::make_unique<AssetBank>();

	myCurrentDirectory = fs::current_path();
	myCurrentDirectory = myCurrentDirectory.parent_path();
	myCurrentDirectory += "/EngineAssets";

	auto& tm = Tga::Engine::GetInstance()->GetTextureManager();

	myFolderTexture = tm.GetTexture(L"Editor/Icons/folder.dds");
	myDocumentTexture = tm.GetTexture(L"Editor/Icons/document.dds");
	myUnknownTexture = tm.GetTexture(L"Editor/Icons/hi_there.dds");
}
Tga::AssetBrowser::~AssetBrowser()
{

}
bool Tga::AssetBrowser::Update(EditorContext& /*aContext*/)
{
	if (!myIsOpen) return false;

	std::string lable = "AssetBrowser##" + std::to_string(myID);
	if (ImGui::Begin(lable.c_str(), &myIsOpen))
	{
		// Top UI
		{
			ImGui::Text("Path: %s", myCurrentDirectory.generic_string().c_str());
			if (ImGui::Button("Back"))
			{
				myCurrentDirectory = myCurrentDirectory.parent_path();

				if (myCurrentDirectory.generic_string().size() <= 4)
				{
					myCurrentDirectory = fs::current_path();
				}
			}
		}

		std::unordered_map<fs::path, Tga::Asset*>& assetBank = ASSET_BANK().GetAssets();
		// Entries
		int itr = 0;
		int entryItr = 0;
		float contentavail = ImGui::GetWindowSize().x;
		float itemSpacing = ImGui::GetStyle().ItemSpacing.x * 2;
		static ImVec2 displaySize = { 90,90 };
		for (auto entry : fs::directory_iterator(myCurrentDirectory))
		{
			Tga::Asset* asset = assetBank[entry.path()];

			// Create new Asset
			if (asset == nullptr)
			{
				asset = CreateAsset(entry);

				//Register to bank
				assetBank[entry.path()] = asset;
			}

			assert(asset != nullptr && "Asset did not load correctly!"); // change for error handler

			ImGui::PushID(++entryItr);
			// Display Item
			if (asset->assetTexture)
			{
				if (ImGui::ImageButton(asset->assetTexture->GetShaderResourceView(), displaySize))
				{
					if (asset->assetType == eAssetType::eFolder)
					{
						myCurrentDirectory = asset->absolutePath;
					}
					//return the path value
				}
			}
			else
			{
				if (ImGui::ImageButton("##", nullptr, displaySize))
				{
					//return the path value
				}
			}
			
			// Tooltip
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text(asset->absolutePath.generic_string().c_str());

				//if (asset->assetType == eAssetType::eFBX)
				//{
				//	//ImGui::Image(AR::GlobalPhoto::GetPhotographer()->CreateRotatingImageOfModel(asset->relativePath.generic_wstring()), ImVec2(250, 250));
				//}

				ImGui::EndTooltip();
			}
			
			// Drag and Drop
			if (asset->assetType != eAssetType::eFolder && ImGui::BeginDragDropSource())
			{
				if (ImGui::SetDragDropPayload("Asset", asset, sizeof(Asset)))
				{
				}
				ImGui::SetTooltip(asset->relativePath.generic_string().c_str());
				ImGui::EndDragDropSource();
			}

			// Write Text over Image
			ImVec2 folderPos = ImVec2();
			if (asset->assetType == eAssetType::eFolder)
			{
				folderPos = ImGui::GetItemRectMin(); // should be where the item ended
				folderPos.x -= ImGui::GetWindowPos().x - itemSpacing;

				std::string overlayText = asset->absolutePath.generic_string();
				int itr2 = (int)overlayText.find_last_of('/') + 1;
				overlayText.erase(overlayText.begin(), overlayText.begin() + itr2);
				overlayText.resize(itr2);

				ImGui::SameLine();
				ImGui::SetCursorPosX(folderPos.x);

				ImGui::Text(overlayText.c_str());
			}

			ImGui::PopID();
			// [Item Spacing]
			{
				if ((itr + 1) * (itemSpacing + displaySize.x) <= contentavail - displaySize.x)
				{
					if (asset->assetType == eAssetType::eFolder)
					{
						float displaySizeX = displaySize.x;
						folderPos.x += displaySizeX;

						ImGui::SameLine();
						ImGui::SetCursorPosX(folderPos.x);
					}
					else
					{
						ImGui::SameLine();
					}

					itr++;
				}
				else
				{
					itr = 0;
				}
				//id++;
			}
		}
		
		// Alter Asset Display Size
		if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) && ImGui::GetIO().MouseWheel != 0)
		{
			//clamp this mf!
			displaySize.x += ImGui::GetIO().MouseWheel * 15.f;

			displaySize.x = std::clamp(displaySize.x, 10.f, 300.f);

			displaySize.y = displaySize.x;

		}
	}
	ImGui::End();

	return myIsOpen;
}

Tga::Asset* Tga::AssetBrowser::CreateAsset(fs::directory_entry aEntry)
{
	Tga::Asset* asset = new Asset();

	//Define Asset Attributes
	asset->absolutePath = aEntry.path();
	asset->relativePath = path_with_forward_slashes(fs::relative(asset->absolutePath, fs::current_path()));

	//Determine Type
	struct stat s;
	stat(asset->absolutePath.generic_string().c_str(), &s);

	if (s.st_mode & S_IFDIR)
	{
		//is folder
		asset->assetType = Tga::eAssetType::eFolder;
		asset->assetTexture = myFolderTexture;
	}
	else if (s.st_mode & S_IFREG)
	{
		// it's a file
		if (IsReadableTexture(asset->relativePath.generic_string()))
		{
			asset->assetTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture(asset->relativePath.c_str());
			asset->assetType = eAssetType::eTexture;
		}
		else if (IsFbxModel(asset->relativePath.generic_string()))
		{
			//Insert get fbx image :skull_emoji:

			/*AR::Photographer* photg = AR::GlobalPhoto::GetPhotographer();*/

			//asset->assetTexture = photg->CreateTextureOfModel(asset->relativePath.generic_wstring());
			asset->assetType = eAssetType::eFBX;
		}
		else
		{
			asset->assetTexture = myDocumentTexture;
		}
	}
	else
	{
		//something else
		asset->assetType = Tga::eAssetType::eUnknown;
		asset->assetTexture = myUnknownTexture;
	}

	return asset;
}
