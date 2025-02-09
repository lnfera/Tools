#pragma once
#include <vector>

namespace Tga
{
	class GameObject;
	class Selection
	{
	public:
		static std::vector<GameObject*> GetSelection();

		static void SetSelection(std::vector<GameObject*> aList);

		static void ClearSelection();
		static void AddToSelection(GameObject* aObject);

		static void RemoveFromSelection(GameObject* aObject);
		static bool Contains(GameObject* aObject);

		static void ToggleSelect(GameObject* aObject);
	private:
	};
};
