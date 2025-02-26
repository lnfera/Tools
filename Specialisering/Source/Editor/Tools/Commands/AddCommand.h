#pragma once
#include "AbstractCommand.h"
#include <Object/GameObject.h>
#include <vector>
namespace Tga
{
	class Scene;
	class GameObject;
	class AddCommand : public AbstractCommand
	{
	public:
		AddCommand(Scene* aScene, std::vector<GameObject*> aList) : mySceneToAddTo(aScene), myObjectsToAdd(aList) {}

		void Execute() override;
		void Undo() override;
		~AddCommand() override;
	private:
		bool myDidPerformAction = true;
		Scene* mySceneToAddTo = nullptr;
		std::vector<GameObject*>myObjectsToAdd;
	};
}