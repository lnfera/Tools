#pragma once
#include <Object/GameObject.h>
namespace Tga
{
	class CopyTool
	{
	public:
		static void ClearCopy();
		static void SetCopyTarget(std::vector<GameObject*> aObjectCopyList);
		static std::vector<GameObject*> CloneCopies();
	private:
	};
}
