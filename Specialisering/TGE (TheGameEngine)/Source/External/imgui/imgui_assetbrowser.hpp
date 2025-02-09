#pragma once
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*																																									*
*																		ImGui AssetBrowser															*
*																																									*
*			Runs on DX11 and requires a way of reading different textures like stb-master for example.		*
*																																									*
*																																									*
*																																									*
*																																									*
*																																									*
*																																									*
*																														by Emil Lehtola :	2024		*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ==============================================================================================
*																Set-Up
* ==============================================================================================
*				Step 1:
				Create a class that contains the asset browser.										
*																																		
*				class GameWorld
*				{
*				public:
*					GameWorld(); 
*					~GameWorld();
*					
*					void Init();
*					void Update(float aTimeDelta); 
*					void Render();
*				
*				private:
*
*					ImGui::AssetBrowser* myBrowser;
* 
*					std::string myAbsPath;
*					std::string myRelPath;
*					ID3D11ShaderResourceView* myTexture;
*				
*				
*				};
* ==============================================================================================
*				Step 2:
*				When starting the application create a new assetbrowser. Create a function pointer in the constructor that 
*				uses your texture manager system. You want to get the shader resource view so if you can set it up like in the example 
*				below.
*
*				 myBrowser = new ImGui::AssetBrowser(
*					[](const wchar_t* textureName) -> ID3D11ShaderResourceView*
*					{
*						// A function that gets the a shader resource view
*						auto texture = Engine::GetInstance()->GetTextureManager().GetTexture(textureName);
*						if (texture)
*						{
*							return texture->GetShaderResourceView();
*						}
*						return nullptr; // Handle cases where texture is not found
*					}
*				);
*				
* ==============================================================================================
*
*				Step 3:
* 				Setup the visuals for the folders if you want them to display in any particular order.
* 
*				myBrowser->SetFolderTexture(L"Sprites/folder_icon.png");
*				myBrowser->SetDocumentTexture(L"Sprites/document_icon.png");
*				myBrowser->SetUnknownTexture(L"Sprites/question_mark.png");
* ==============================================================================================
*			
				Step 4:
*				Somewhere in your game loop setup the display function and wherever you want the drag and drop function to accept 
*				you can use the following code as an example of what you can get:
* 
*				//Display function
* 				myBrowser->Display();
*
* 
*				//Example code
*				if (ImGui::Begin("Debug"))
*				{
*				    if (ImGui::BeginDragDropTarget())
*				    {
*				        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("Asset"))
*				        {
*
*				            ImGui::Asset* droppedAsset = reinterpret_cast<ImGui::Asset*>(p->Data);
*
*				            myTexture = droppedAsset->texture;
*				            myRelPath = droppedAsset->relativePath.generic_string();
*				            myAbsPath = droppedAsset->absolutePath.generic_string();
*
*				        }
*
*				        ImGui::EndDragDropTarget();
*				    }
*
*				}
*				ImGui::End();
* 
*			TO DO
*
*			Vilka bilder får läsas?
*			Set path addition for getting tex
*			Controller med ctrl + scroll wheel
*
*
*
*/

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <functional>
#include <d3d11.h>
#include "imgui.h"
static std::function<ID3D11ShaderResourceView* (const wchar_t*)> FindTextureFunc = nullptr;
static ID3D11ShaderResourceView* gFolderTexture = nullptr;
static ID3D11ShaderResourceView* gDocumentTexture = nullptr;
static ID3D11ShaderResourceView* gUnknownTexture = nullptr;
static ImVec2 gDisplaySize = { 50,50 };

namespace  fs = std::filesystem;

namespace ImGui
{
	enum class eAssetType
	{
		undefined = 0,
		Folder = 1,
		Texture = 2,
		Document = 3,
		Unknown = 4,
		COUNT
	};

	struct Asset
	{
		ID3D11ShaderResourceView* texture;
		fs::path absolutePath;
		fs::path relativePath;
		eAssetType type = eAssetType::undefined;
	};

	class AssetBrowser
	{
	public:
		AssetBrowser(std::function<ID3D11ShaderResourceView* (const wchar_t*)> aFunction)
		{
			FindTextureFunc = aFunction;
			myCurrentDirectory = fs::current_path();
		}
		ID3D11ShaderResourceView* GetSRV(const wchar_t* aPath)
		{
			return FindTextureFunc(aPath);
		}
		inline void Display();

		inline void Open() { myIsOpen = true; }
		inline void Close() { myIsOpen = false; }

		inline void SetFolderTexture(const wchar_t* aPath) { gFolderTexture = FindTextureFunc(aPath); }
		inline void SetDocumentTexture(const wchar_t* aPath) { gDocumentTexture = FindTextureFunc(aPath); }
		inline void SetUnknownTexture(const wchar_t* aPath) { gUnknownTexture = FindTextureFunc(aPath); }

		//inline void AddReadableTextureType(eTextureType aType) { myReadableTextures |= aType; };

	private:

		inline void GoBack();
		inline bool IsReadableTexture(std::string aPath);

		bool myIsOpen = true;
		std::unordered_map<fs::path, Asset*> myAssets;
		std::vector<Asset*> myBank;
		fs::path myCurrentDirectory;
	};
}
inline void ImGui::AssetBrowser::GoBack()
{
	std::string path = myCurrentDirectory.generic_string();

	int itr = (int)path.find_last_of("/");
	path.resize(itr);
	myCurrentDirectory = path;
}

inline bool ImGui::AssetBrowser::IsReadableTexture(std::string aPath)
{
	if (aPath.find(".dds") != -1)
	{
		return true;
	}
	else if (aPath.find(".png") != -1)
	{
		return true;
	}
	else if (aPath.find(".jpg") != -1)
	{
		return true;
	}

	return false;
}

inline void ImGui::AssetBrowser::Display()
{
	if (!myIsOpen) return;
	if (ImGui::Begin("AssetBrowser", &myIsOpen))
	{
		ImGui::Text("Path: %s",myCurrentDirectory.generic_string().c_str());

		if (ImGui::Button("Back"))
		{
			GoBack();
		}

		int itr = 0;
		int id = 0;
		float itemWidth = (ImGui::GetStyle().ItemSpacing.x * 2) + gDisplaySize.x;
		float contentavail = ImGui::GetContentRegionAvail().x;
		//Renders each item in directory
		for (auto entry : fs::directory_iterator(myCurrentDirectory))
		{
			Asset* asset = myAssets[entry.path()];
			ImGui::PushID(id);
			if (asset == nullptr)
			{
				//Create Asset
				myAssets[entry.path()] = new Asset();
				asset = myAssets[entry.path()];
				//Define Asset attributes path etc
				asset->absolutePath = entry.path();
				asset->relativePath = fs::relative(asset->absolutePath, fs::current_path());


				auto status = entry.status();

				if (status.type() == fs::file_type::directory)
				{
					asset->type = eAssetType::Folder;
					asset->texture = gFolderTexture;
				}
				else if (status.type() == fs::file_type::regular)
				{
					std::string path = entry.path().generic_string();
					if (IsReadableTexture(path))
					{
						asset->type = eAssetType::Texture;
						asset->texture = FindTextureFunc(asset->relativePath.c_str());
					}

				}
				else
				{
				}
			}

			//Render Asset
			if (ImGui::ImageButton(asset->texture, gDisplaySize))
			{
				if (asset->type == eAssetType::Folder)
				{
					myCurrentDirectory = asset->absolutePath;
				}
				//return the path value
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(asset->absolutePath.generic_string().c_str());
			}

			if (asset->type != eAssetType::Folder&&ImGui::BeginDragDropSource())
			{
				if (ImGui::SetDragDropPayload("Asset", asset, sizeof(Asset)))
				{
				}
				ImGui::SetTooltip(asset->relativePath.generic_string().c_str());
				ImGui::EndDragDropSource();
			}


			if (itr * itemWidth <= contentavail)
			{
				ImGui::SameLine();
			}
			else
			{
				itr = 0;
			}
			itr++;
			id++;
			ImGui::PopID();
		}

		//Controlls

	}
	ImGui::End();
}
