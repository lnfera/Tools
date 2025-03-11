#include "CopyTool.h"
#include <Tools/ConsoleSystem.h>
static std::vector<Tga::GameObject*> pastedObjects;


void Tga::CopyTool::ClearCopy()
{
	for (int i = 0; i < pastedObjects.size(); i++)
	{
		delete pastedObjects[i];
	}
	pastedObjects.clear();
}

void Tga::CopyTool::SetCopyTarget(std::vector<GameObject*> aObjectList)
{
	ClearCopy();

	for (int i = 0; i < aObjectList.size(); i++)
	{
		pastedObjects.push_back(new GameObject(aObjectList[i]));
	}


	std::string message = std::to_string(pastedObjects.size()) + " objects copied";
	Tga::Console::LogMessage(message, Tga::Console::Colors::White);
}

std::vector<Tga::GameObject*> Tga::CopyTool::CloneCopies()
{
	std::vector<GameObject*> list;
	for (auto* obj : pastedObjects)
	{
		GameObject* addedObj = new GameObject(obj);
		list.push_back(addedObj/*AR::Engine::GetInstance()->GetGameFactory().Create(obj)*/);
	}
	std::string message = std::to_string(pastedObjects.size()) + " objects pasted!";
	Tga::Console::LogMessage(message, Tga::Console::Colors::White);

	return list;
}
