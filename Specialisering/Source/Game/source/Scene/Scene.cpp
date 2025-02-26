#include "Scene.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <tge/engine.h>
#include "../Object/GameObject.h"
#include <Object/I_AllComponents.h>
Tga::Scene::Scene(std::string aFilename) :
	myFileName(aFilename)
{
}

Tga::Scene::~Scene()
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		delete myGameObjects[i];
	}
}

void Tga::Scene::Init()
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		myGameObjects[i]->Init();
	}
}

void Tga::Scene::Update(float aDeltaTime)
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		myGameObjects[i]->Update(aDeltaTime);
	}
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		myGameObjects[i]->UpdateComponents(aDeltaTime);
	}
}

void Tga::Scene::Render()
{
}

void Tga::Scene::Enable()
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		myGameObjects[i]->Enable();
	}
}

void Tga::Scene::Disable()
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		myGameObjects[i]->Disable();
	}
}

void Tga::Scene::SaveToJson(std::string aFileName)
{
	nlohmann::json jsonData;
	// Pack Data
	{
		//Save Objects
		for (int i = 0; i < myGameObjects.size(); i++)
		{
			jsonData["Objects"].push_back(myGameObjects[i]->SaveData());
		}

		// Save Scrits
	}

	if (aFileName == " ")
	{
		aFileName = myFileName;
	}
	myFileName = aFileName;

	// Save Data
	{
		std::ofstream outFile("Json/" + aFileName + ".json");
		if (outFile.is_open())
		{
			outFile << jsonData.dump(4);

			outFile.close();
		}
		else
		{
			throw("Could not open file!");
		}
	}
}

void Tga::Scene::LoadFromJson(std::string aFilePath)
{
	if (aFilePath.find(".json") != -1)
	{
		aFilePath.erase(aFilePath.begin() + aFilePath.find_first_of("."), aFilePath.end());
	}

	myFileName = aFilePath;

	nlohmann::json jsonData;
	//Load Data
	{
		//Save to file
		std::ifstream inFile("Json/" + myFileName + ".json");
		if (inFile.is_open())
		{
			inFile >> jsonData;
			inFile.close();
		}
		else
		{
			throw("Could not open file!");
		}
	}

	//auto& engine = *Tga::Engine::GetInstance();
	// UnPack Data
	{
		ClearObjects();

		int arraySize = (int)jsonData["Objects"].size();
		for (int i = 0; i < arraySize; i++)
		{
			nlohmann::json objData = jsonData["Objects"][i];

			GameObject* object = new GameObject(/*engine.GetUniqueID()*/);

			//Loads basics
			object->LoadData(objData);

			//Loads components
			int componentAmount = (int)objData["Components"].size();
			for (int k = 0; k < componentAmount; k++)
			{
				std::string type = objData["Components"][k]["Type"];

				auto* component = CreateComponentFromType(type, object);

				if (component == nullptr)
				{
					continue;
				}

				component->LoadData(objData["Components"][k]);

				//object->AddComponent(component);
			}

			for (auto* component : object->GetComponentList())
			{
				component->Init();
			}

			myGameObjects.push_back(object);
		}
	}
}

void Tga::Scene::RemoveObject(GameObject* aObject)
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		if (myGameObjects[i] == aObject)
		{
			myGameObjects.erase(myGameObjects.begin() + i);
			return;
		}
	}
}

void Tga::Scene::ClearObjects()
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		delete myGameObjects[i];
	}
	myGameObjects.clear();
}
Tga::GameObject* Tga::Scene::GetObjectWithID(unsigned int aId)
{
	for (int i = 0; i < myGameObjects.size(); i++)
	{
		if (myGameObjects[i]->GetID() == aId) {
			return myGameObjects[i];
		}
	}

	return nullptr;
}

