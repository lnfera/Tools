#pragma once
#include "Component.h"
#include "Components/RenderComponent.h"

#pragma warning( push )
#pragma warning(suppress: 4505)
static Tga::Component* CreateComponentFromType(std::string aType, Tga::GameObject* aObject)
{
	aObject;
	aType.erase(aType.begin(), aType.begin() + 6);

	if (aType == "Tga::RenderComponent")
	{
		return new Tga::RenderComponent(aObject);
	}

	return nullptr;
}
#pragma warning (pop)