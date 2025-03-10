#include "RemoveCommand.h"
#include <Object/GameObject.h>
#include <Scene/Scene.h>
void Tga::RemoveCommand::Execute()
{
	myDidPerformAction = true;
	for (int i = 0; i < myObjectsToRemove.size(); i++)
	{
		myObjectsToRemove[i]->Disable();
		mySceneToRemoveFrom->RemoveObject(myObjectsToRemove[i]);
	}
}

void Tga::RemoveCommand::Undo()
{
	myDidPerformAction = false;
	for (int i = 0; i < myObjectsToRemove.size(); i++)
	{
		myObjectsToRemove[i]->Enable();
		mySceneToRemoveFrom->AddObject(myObjectsToRemove[i]);
	}
}

Tga::RemoveCommand::~RemoveCommand()
{
	if (myDidPerformAction == true)
	{
		for (int i = 0; i < myObjectsToRemove.size(); i++)
		{
#pragma warning(suppress : 4006)
			delete myObjectsToRemove[i];
		}
	}
}
