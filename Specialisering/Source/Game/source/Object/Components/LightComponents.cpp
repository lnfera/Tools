#include "LightComponents.h"

#include <tge/graphics/PointLight.h>

#include <Core/MainDrawer.h>
#include <Core/MainSingleton.h>

#include <imgui/imgui.h>
#include "../GameObject.h"
Tga::PointLightComponent::PointLightComponent(GameObject* aObject) :
	Component(aObject)
{
	myPointLight = new PointLight();
}

Tga::PointLightComponent::~PointLightComponent()
{
	delete myPointLight;
}

Tga::Component* Tga::PointLightComponent::Clone(GameObject* aObject)
{
	Tga::PointLightComponent* clone = new Tga::PointLightComponent(aObject);
	clone->myPointLight->SetColor(myPointLight->GetColor());
	clone->myPointLight->SetIntensity(myPointLight->GetIntensity());
	clone->myPointLight->SetRange(myPointLight->GetRange());
	return nullptr;
}

void Tga::PointLightComponent::Update(float /*aDeltaTime*/)
{
	myPointLight->SetTransform(myGameObject->GetTransform());
}

void Tga::PointLightComponent::ImGuiAccess()
{
	auto color = myPointLight->GetColor();
	if (ImGui::ColorPicker4("Color", &color.r))
	{
		myPointLight->SetColor(color);
	}
	float range = myPointLight->GetRange();
	if (ImGui::DragFloat("Range", &range, 1.f))
	{
		myPointLight->SetRange(range);
	}
	float intesity = myPointLight->GetIntensity();
	if (ImGui::DragFloat("Range", &intesity, 10.f))
	{
		myPointLight->SetIntensity(intesity);
	}
}

NM::json Tga::PointLightComponent::SaveData() const
{
	NM::json json;
	auto color = myPointLight->GetColor();
	json["Color"]["R"] = color.r;
	json["Color"]["G"] = color.g;
	json["Color"]["B"] = color.b;
	json["Color"]["A"] = color.a;

	json["Intensity"] = myPointLight->GetIntensity();
	json["Range"] = myPointLight->GetRange();
	return NM::json();
}

void Tga::PointLightComponent::LoadData(NM::json aJsonData)
{
	Tga::Color color = myPointLight->GetColor();

	color.r = aJsonData["Color"]["R"];
	color.g = aJsonData["Color"]["G"];
	color.b = aJsonData["Color"]["B"];
	color.a = aJsonData["Color"]["A"];

	myPointLight->SetIntensity(aJsonData["Intesity"]);
	myPointLight->SetRange(aJsonData["Range"]);
}

void Tga::PointLightComponent::Enable()
{
	Tga::MainSingleton::GetInstance().GetMainDrawer()->AddLight(myPointLight);
}

void Tga::PointLightComponent::Disable()
{
	Tga::MainSingleton::GetInstance().GetMainDrawer()->RemoveLight(myPointLight);
}
