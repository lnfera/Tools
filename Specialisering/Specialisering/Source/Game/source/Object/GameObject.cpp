#include "GameObject.h"

nlohmann::json Tga::GameObject::SaveData() const
{
	Vector3f pos, scale;
	Quaternionf rot;

	myTransform.DecomposeMatrix(pos, rot, scale);


	nlohmann::json result;
	result["name"] = myName;

	for (int row = 1; row < 5; row++)
	{
		for (int col = 1; col < 5; col++)
		{
			result["tranform"][row][col] = myTransform(row, col);
		}
	}


	for (int i = 0; i < myComponents.size(); i++)
	{
		
		std::string type = typeid(*myComponents[i]).name();
		type.erase(type.begin(), type.begin() + 6);


		result["Components"][i] = myComponents[i]->SaveData();
		result["Components"][i]["Type"] = type;
	}

	//Sent to Scene.cpp -> SaveScene
	return result;
}

void Tga::GameObject::LoadData(const nlohmann::json& aJsonData)
{
	myName = aJsonData["name"];

	for (int row = 1; row < 5; row++)
	{
		for (int col = 1; col < 5; col++)
		{
			myTransform(row, col) = aJsonData["tranform"][row][col];
		}
	}
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
