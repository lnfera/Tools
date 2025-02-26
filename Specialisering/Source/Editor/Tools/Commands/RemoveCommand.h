#pragma once
#include "AbstractCommand.h"
#include <Object/GameObject.h>
#include <vector>
namespace Tga
{
	class Scene;
	class GameObject;
	class RemoveCommand : public AbstractCommand
	{
	public:
		RemoveCommand(Scene* aScene, std::vector<GameObject*> aList) : mySceneToRemoveFrom(aScene), myObjectsToRemove(aList) {}

		void Execute() override;
		void Undo() override;

		~RemoveCommand() override;
	private:
		bool myDidPerformAction = true;
		Scene* mySceneToRemoveFrom = nullptr;
		std::vector<GameObject*> myObjectsToRemove;
	};
}