#pragma once
#include <nlohmann/json.hpp>

namespace NM = nlohmann;

namespace Tga
{
	class GameObject;
	class Component
	{
	public:
		Component(GameObject* aObject);
		virtual ~Component() {};

		virtual Component* Clone(GameObject*) = 0;

		virtual void Init() { __noop; }
		virtual void Update(float /*aDeltaTime*/) { __noop; }

		virtual void Enable() { __noop; }
		virtual void Disable() { __noop; }

		//Editor functions
		virtual void ImGuiAccess(){	__noop;}

		virtual NM::json SaveData() const
		{
			NM::json result;

			return result;
		}
		virtual void LoadData(NM::json /*aJsonData*/){__noop;}

		GameObject* GetGameObject() { return myGameObject; }
	protected:
		GameObject* myGameObject = nullptr;
	};
}