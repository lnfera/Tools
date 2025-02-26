#include "AddCommand.h"
#include "CommandManager.h"
#include <Scene/Scene.h>
void Tga::AddCommand::Execute()
{
	myDidPerformAction = true;
	for (int i = 0; i < myObjectsToAdd.size(); i++)
	{
		mySceneToAddTo->AddObject(myObjectsToAdd[i]);
	}
}

void Tga::AddCommand::Undo()
{
	myDidPerformAction = false;

	for (int i = 0; i < myObjectsToAdd.size(); i++)
	{
		mySceneToAddTo->RemoveObject(myObjectsToAdd[i]);
	}
}

Tga::AddCommand::~AddCommand()
{
	if (myDidPerformAction == false)
	{
		for (int i = 0; i < myObjectsToAdd.size(); i++)
		{
#pragma warning(suppress : 4006)
			delete myObjectsToAdd[i];
		}
	}
}
