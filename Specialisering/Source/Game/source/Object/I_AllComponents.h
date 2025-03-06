#pragma once
#include "Component.h"
#include "Components/RenderComponent.h"
#include "Components/LightComponents.h"

#pragma warning( push )
#pragma warning(suppress: 4505)
static Tga::Component* CreateComponentFromType(std::string aType, Tga::GameObject* aObject)
{

	if (aType == "Tga::RenderComponent")
	{
		return new Tga::RenderComponent(aObject);
	}

	else if (aType == "Tga::PointLightComponent")
	{
		return new Tga::PointLightComponent(aObject);
	}

	return nullptr;
}
#pragma warning (pop)