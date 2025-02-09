#pragma once
#include <vector>
#include <iostream>
namespace Tga
{
	class GameObject;
	class UiElement;
}

namespace Tga
{
	class Scene // listener
	{
	public:
		Scene(std::string aFilename);
		virtual ~Scene();
		virtual void Init();
		virtual void Update(float aDeltaTime);
		virtual void Render();

		virtual void Enable();
		virtual void Disable();
	public:
		void SaveToJson(std::string aFileName = " ");
		void LoadFromJson(std::string aFilePath);

		std::string GetFileName() { return myFileName; };
		void SetFileName(std::string aFileName) { myFileName = aFileName; }

		void AddObject(GameObject* aObject) { myGameObjects.push_back(aObject); };
		void RemoveObject(GameObject* aObject);
		std::vector<GameObject*>& GetGameObjects() { return myGameObjects; }
		void ClearObjects();

	/*	void UpdateUI();
		void ClearUI();
		void AddUiElement(UiElement* aUiElement) { myUiElements.push_back(aUiElement); };
		std::vector<UiElement*>& GetUiElements() { return myUiElements; }*/
	private:
		std::string myFileName = "undefined";
		std::vector<GameObject*> myGameObjects;
		//std::vector<UiElement*> myUiElements;
	};
}