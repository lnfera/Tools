#include "RemoveCommand.h"
#include <Object/GameObject.h>
#include <Scene/Scene.h>
void Tga::RemoveCommand::Execute()
{
	myDidPerformAction = true;
	for (int i = 0; i < myObjectsToRemove.size(); i++)
	{
		mySceneToRemoveFrom->RemoveObject(myObjectsToRemove[i]);
	}
}

void Tga::RemoveCommand::Undo()
{
	myDidPerformAction = false;
	for (int i = 0; i < myObjectsToRemove.size(); i++)
	{
		mySceneToRemoveFrom->AddObject(myObjectsToRemove[i]);
	}
}

Tga::RemoveCommand::~RemoveCommand()
{
	if (myDidPerformAction == false)
	{
		for (int i = 0; i < myObjectsToRemove.size(); i++)
		{
#pragma warning(suppress : 4006)
			delete myObjectsToRemove[i];
		}
	}
}
