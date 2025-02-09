#include "GameObject.h"

nlohmann::json Tga::GameObject::SaveData() const
{
	nlohmann::json result;
	result["name"] = myName;

	Tga::Vector3f pos = myTransform.GetPosition();
	result["pos"] = { pos.x,pos.y,pos.z };
	Tga::Vector3f scale = myTransform.GetScale();
	result["scale"] = { scale.x,scale.y,scale.z };

	// rotation

	for (int i = 0; i < myComponents.size(); i++)
	{
		result["Components"][i] = myComponents[i]->SaveData();
	}

	//Sent to Scene.cpp -> SaveScene
	return result;
}

void Tga::GameObject::LoadData(const nlohmann::json& aJsonData)
{
	myName = aJsonData["name"];
	myTransform.SetPosition({ aJsonData["pos"][0],aJsonData["pos"][1],aJsonData["pos"][2] });
	myTransform.SetScale({ aJsonData["scale"][0],aJsonData["scale"][1],aJsonData["scale"][2] });

	// Components are loaded in Scene.cpp -> LoadScene() due to platform compatability
}

void Tga::GameObject::RemoveComponent(Component* aComponent)
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i] == aComponent)
		{
			myComponents[i] = nullptr;
			myComponents.erase(myComponents.begin() + i);
			return;
		}
	}
}
