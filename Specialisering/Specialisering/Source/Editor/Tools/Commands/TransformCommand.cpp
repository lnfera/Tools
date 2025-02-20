#include "TransformCommand.h"

//Game
#include <Object/GameObject.h>
//Editor
#include "CommandManager.h"
void Tga::TransformCommand::Execute()
{
	for (int i = 0; i < myEndTransforms.size(); i++)
	{
		myObjectsToMove[i]->GetTransform() = myEndTransforms[i];
	}
}

void Tga::TransformCommand::Undo()
{
	for (int i = 0; i < myBeginObjects.size(); i++)
	{
		myObjectsToMove[i]->GetTransform() = myBeginObjects[i];
	}
}

Tga::TransformCommand::~TransformCommand()
{
	myBeginObjects.clear();
	myEndTransforms.clear();
}

void Tga::TransformCommand::BeginCommand(std::vector<GameObject*> aGroupOfObjects)
{
	myObjectsToMove = aGroupOfObjects;
	myBeginObjects.clear();
	for (auto* obj : aGroupOfObjects)
	{
		myBeginObjects.push_back(obj->GetTransform());
	}
}

void Tga::TransformCommand::EndCommand()
{
	myEndTransforms.clear();
	for (auto* obj : myObjectsToMove)
	{
		myEndTransforms.push_back(obj->GetTransform());
	}
	Tga::CommandManager::DoCommand(std::make_shared<TransformCommand>(*this));
	*this = TransformCommand();
}
