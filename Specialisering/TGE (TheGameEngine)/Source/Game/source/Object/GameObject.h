#pragma once
#include <vector>
#include <tge/math/Transform.h>
#include <nlohmann/json.hpp>
#include "Component.h"
namespace Tga
{
	enum class eObjectTag
	{
		DEFAULT = 0,
		PLAYER,
		COUNT
	};

	class GameObject/* : public Listener*/
	{
	public:
		class GameObject(unsigned int aID) : myID(aID) {};
		virtual ~GameObject()
		{
			for (int i = 0; i < myComponents.size(); i++)
			{
				delete myComponents[i];
			}
			myComponents.clear();
		}

		void Enable();
		void Disable();

		virtual void Init() { __noop; }
		virtual void GameObject::Update(float /*aDeltaTime*/)
		{
			__noop;
		}
		void GameObject::UpdateComponents(float aDeltaTime)
		{
			for (auto* component : myComponents)
			{
				component->Update(aDeltaTime);
			}
		}

		nlohmann::json SaveData() const;
		void LoadData(const nlohmann::json& aJsonData);

		template <typename T>
		void AddComponent();
		void AddComponent(Component* aComponent) { myComponents.push_back(aComponent); };
		template <typename T>
		T* GetComponent();
		const std::vector<Component*>& GetComponentList() { return myComponents; }
		void RemoveComponent(Component* aComponent);


		const unsigned int GetID() { return myID; }
		eObjectTag GetObjectTag() { return myObjectTag; }
		Transform& GetTransform() { return myTransform; }
		std::string GetName() { return myName; }
		void SetName(std::string aName) { myName = aName; }
	protected:
		const unsigned int myID;
		eObjectTag myObjectTag;
		std::string myName = "";
		std::vector<Component*> myComponents;
		Transform myTransform;
	};

	inline void GameObject::Enable()
	{
		for (auto* component : myComponents)
		{
			component->Enable();
		}
	}
	inline void GameObject::Disable()
	{
		for (auto* component : myComponents)
		{
			component->Disable();
		}
	}

#pragma region Components
	template<typename T>
	inline void GameObject::AddComponent()
	{
		new T(this);
	}
	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (Tga::Component* component : myComponents)
		{
			if (component == nullptr)
			{
				// assert
			}

			T* comp = dynamic_cast<T*>(component);

			if (comp != nullptr)
			{
				return comp;
			}
		}

		return nullptr;
	}
#pragma endregion
}