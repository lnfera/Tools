#include "Component.h"
#include "GameObject.h"
Tga::Component::Component(GameObject* aObject) 
	: myGameObject(aObject) 
{
	myGameObject->AddComponent(this);
};
