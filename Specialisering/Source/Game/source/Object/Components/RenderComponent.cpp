#include "RenderComponent.h"
#include <tge/engine.h>
#include <tge/model/ModelFactory.h>
#include <tge/model/ModelInstance.h>
#include <Object/GameObject.h>
#include <tge/texture/TextureManager.h>
#include <tge/shaders/shader.h>

#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/ModelDrawer.h>
#include <imgui/imgui.h>
#include <Core/MainSingleton.h>
#include <Core/MainDrawer.h>
#include <Shader/ShaderManager.h>

#include <tge/util/Asset.h>
#include <tge/util/StringCast.h>

void ImageDrop(char* aLable , Tga::ModelInstance* aInstance, int aSlot , std::string& aPath)
{
	ImGui::Text(aLable);

	const Tga::TextureResource* currentTexture = aInstance->GetTextures(0)[aSlot];
	ImVec2 size(50,50);
	if (currentTexture == nullptr)
	{
		ImGui::Image((ImTextureID)nullptr, size);
	}
	else
	{
		ImGui::Image((ImTextureID)currentTexture->GetShaderResourceView(), size);
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (auto p = ImGui::AcceptDragDropPayload("Asset"))
		{
			if (Tga::Asset* asset = reinterpret_cast<Tga::Asset*>(p->Data))
			{
				aInstance->SetTexture(0, aSlot, asset->assetTexture);
				aPath = asset->relativePath.generic_string();
				
			}
		}
		ImGui::EndDragDropTarget();
	}
}

Tga::RenderComponent::RenderComponent(GameObject* aObject) : Component(aObject)
{
	myModelInstance = new Tga::ModelInstance();
	myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	myModelInstance->SetTexture(0, 0, TGE_I()->GetTextureManager().GetTexture(L"Textures/T_Default_BC.dds"));
	myModelInstance->SetTexture(0, 1, TGE_I()->GetTextureManager().GetTexture(L"Textures/T_Default_N.dds"));
	myModelInstance->SetTexture(0, 2, TGE_I()->GetTextureManager().GetTexture(L"Textures/T_Default_M.dds"));
	myModelInstance->SetTexture(0, 3, TGE_I()->GetTextureManager().GetTexture(L"Textures/T_Default_M.dds"));
}
Tga::RenderComponent::~RenderComponent()
{
	delete myModelInstance;

}

Tga::Component* Tga::RenderComponent::Clone(GameObject* aObj)
{
	RenderComponent* comp = new RenderComponent(aObj);
	comp->myModelInstance = new Tga::ModelInstance();

	comp->myModelInstance->Init(myModelInstance->GetModel());
	comp->myModelInstance->SetTexture(0, 0, const_cast<Tga::TextureResource*>(myModelInstance->GetTexture(0,0)));
	comp->myModelInstance->SetTexture(0, 1, const_cast<Tga::TextureResource*>(myModelInstance->GetTexture(0,1)));
	comp->myModelInstance->SetTexture(0, 2, const_cast<Tga::TextureResource*>(myModelInstance->GetTexture(0,2)));
	comp->myModelInstance->SetTexture(0, 3, const_cast<Tga::TextureResource*>(myModelInstance->GetTexture(0,3)));
	//comp->myModelInstance->SetTexture(0, 0, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
	comp->myShader = myShader;
	comp->myShaderName = myShaderName;
	comp->myTexturePaths[0] = myTexturePaths[0];
	comp->myTexturePaths[1] = myTexturePaths[1];
	comp->myTexturePaths[2] = myTexturePaths[2];
	comp->myTexturePaths[3] = myTexturePaths[3];

	return comp;
}

void Tga::RenderComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	myModelInstance->SetTransform(myGameObject->GetTransform());
}
void Tga::RenderComponent::ImGuiAccess()
{
	const auto& shaderList = MSI_().GetShaderManager()->GetShaderList();
	bool didChangeRenderGroup = false;

	ImGui::Text("Model : %s", myModelInstance->GetModel()->GetMeshName(0));

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		ImGui::Text("Drop models from the asset browser to change model");
		ImGui::NewLine();
		ImGui::Text("Vertex Count : %i", myModelInstance->GetModel()->GetMeshData(0).NumberOfVertices);

		ImGui::EndTooltip();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (auto p = ImGui::AcceptDragDropPayload("Asset"))
		{
			Asset* asset = reinterpret_cast<Asset*>(p->Data);

			if (asset->assetType == Tga::eAssetType::eFBX)
			{
				Disable();
				std::string modelPath = asset->relativePath.generic_string();
				myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(string_cast<std::wstring>(modelPath)));
				didChangeRenderGroup = true;
			}
		}

		ImGui::EndDragDropTarget();
	}


	if (myShader)
	{
		ImGui::Text("Shader: %s", myShaderName.c_str());
	}

	else
	{
		ImGui::Text("Shader: none");

	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Click to change shader");
		if (ImGui::IsItemClicked())
		{
			ImGui::OpenPopup("ShaderList");
		}
	}
	//if (!myShader)
	{
		ImageDrop("Albedo", myModelInstance, 0, myTexturePaths[0]);
		ImageDrop("Normal", myModelInstance, 1, myTexturePaths[1]);
		ImageDrop("Material", myModelInstance, 2, myTexturePaths[2]);
		ImageDrop("FX", myModelInstance, 3, myTexturePaths[3]);
	}



	if (ImGui::BeginPopup("ShaderList"))
	{
		for (auto pair : shaderList)
		{
			if (ImGui::MenuItem(pair.first.c_str()))
			{
				Disable();
				myShader = pair.second;
				myShaderName = pair.first;
				didChangeRenderGroup = true;

			}
		}

		if (ImGui::Button("Clear"))
		{
			myShader = nullptr;
			myShaderName = "";
			didChangeRenderGroup = true;
		}

		ImGui::EndPopup();
	}
	if (didChangeRenderGroup)
	{
		Enable();
	}
}

NM::json Tga::RenderComponent::SaveData() const
{
	NM::json result;
	result["shader"] = myShaderName;
	result["model"] = std::filesystem::relative(myModelInstance->GetModel()->GetPath()).generic_string();

	result["albedo"]	= myTexturePaths[0];
	result["normal"]	= myTexturePaths[1];
	result["material"]	= myTexturePaths[2];
	result["fx"]		= myTexturePaths[3];

	return result;
}
void Tga::RenderComponent::LoadData(NM::json aJsonData)
{
	myShaderName = aJsonData["shader"];

	if (myShaderName.empty() == false)
	{
		myShader = MSI_().GetShaderManager()->GetShader(myShaderName);
	}
	std::string modelPath = aJsonData["model"];


	myTexturePaths[0] = aJsonData["albedo"];
	myTexturePaths[1] = aJsonData["normal"];
	myTexturePaths[2] = aJsonData["material"];
	myTexturePaths[3] = aJsonData["fx"];


	auto& tM = TGE_I()->GetTextureManager();
	
	if (myTexturePaths[0] != "none")
	{
		myModelInstance->SetTexture(0, 0, tM.GetTexture(string_cast<std::wstring>(myTexturePaths[0]).c_str()));
	}
	if (myTexturePaths[1] != "none")
	{
		myModelInstance->SetTexture(0, 1, tM.GetTexture(string_cast<std::wstring>(myTexturePaths[1]).c_str()));
	}
	if (myTexturePaths[2] != "none")
	{
		myModelInstance->SetTexture(0, 2, tM.GetTexture(string_cast<std::wstring>(myTexturePaths[2]).c_str()));
	}
	if (myTexturePaths[3] != "none")
	{
		myModelInstance->SetTexture(0, 3, tM.GetTexture(string_cast<std::wstring>(myTexturePaths[3]).c_str()));
	}
	myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(string_cast<std::wstring>(modelPath)));
}

void Tga::RenderComponent::Enable()
{
	Tga::MainSingleton::GetInstance().GetMainDrawer()->AddModelToRenderGroup(myModelInstance, myShader);
}
void Tga::RenderComponent::Disable()
{
	Tga::MainSingleton::GetInstance().GetMainDrawer()->RemoveModelFromRenderGroup(myModelInstance, myShader);
}
