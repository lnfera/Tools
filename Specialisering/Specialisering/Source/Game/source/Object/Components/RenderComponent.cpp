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
#include <Shader/ShaderManager.h>

#include <tge/util/Asset.h>
#include <tge/util/StringCast.h>


Tga::RenderComponent::RenderComponent(GameObject* aObject) : Component(aObject)
{
	myModelInstance = new Tga::ModelInstance();
	myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	myModelInstance->SetTexture(0, 0, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
	myModelInstance->SetTexture(0, 1, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
	myModelInstance->SetTexture(0, 2, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
}
Tga::RenderComponent::~RenderComponent()
{
	delete myModelInstance;

	if (myShader)
	{
		//delete myShader;
	}
}

Tga::Component* Tga::RenderComponent::Clone(GameObject* aObj)
{
	RenderComponent* comp = new RenderComponent(aObj);
	comp->myModelInstance = new Tga::ModelInstance();

	comp->myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	comp->myModelInstance->SetTexture(0, 0, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));


	return comp;
}

void Tga::RenderComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	myModelInstance->SetTransform(myGameObject->GetTransform());

	if (myShader)
	{
		// Renders with custom shader
		TGE_I()->GetGraphicsEngine().GetModelDrawer().Draw(*myModelInstance, *myShader);
	}
	else
	{
		// Renders with default shader
		TGE_I()->GetGraphicsEngine().GetModelDrawer().Draw(*myModelInstance);
	}
}
void Tga::RenderComponent::ImGuiAccess()
{
	const auto& shaderList = MSI_().GetShaderManager()->GetShaderList();

	ImGui::Text("Model : %s", myModelInstance->GetModel()->GetMeshName(0));

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Drop models from the asset browser to change model");
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (auto p = ImGui::AcceptDragDropPayload("Asset"))
		{
			Asset* asset = reinterpret_cast<Asset*>(p->Data);

			if (asset->assetType == Tga::eAssetType::eFBX)
			{
				std::string modelPath = asset->relativePath.generic_string();
				myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(string_cast<std::wstring>(modelPath)));
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


	if (ImGui::BeginPopup("ShaderList"))
	{
		for (auto pair : shaderList)
		{
			if (ImGui::MenuItem(pair.first.c_str()))
			{
				myShader = pair.second;
				myShaderName = pair.first;
			}
		}

		if (ImGui::Button("Clear"))
		{
			myShader = nullptr;
			myShaderName = "";
		}

		ImGui::EndPopup();
	}
}

NM::json Tga::RenderComponent::SaveData() const
{
	NM::json result;
	result["shader"] = myShaderName;
	result["model"] = std::filesystem::relative(myModelInstance->GetModel()->GetPath()).generic_string();

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

	myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(string_cast<std::wstring>(modelPath)));

}
