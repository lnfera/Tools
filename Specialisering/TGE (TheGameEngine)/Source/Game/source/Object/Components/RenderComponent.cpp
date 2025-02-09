#include "RenderComponent.h"
#include <tge/engine.h>
#include <tge/model/ModelFactory.h>
#include <tge/model/ModelInstance.h>
#include <Object/GameObject.h>
#include <tge/texture/TextureManager.h>
Tga::RenderComponent::RenderComponent(GameObject* aObject) : Component(aObject)
{
	myModelInstance = new Tga::ModelInstance();
	myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	myModelInstance->SetTexture(0,0,TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
	myModelInstance->SetTexture(0,1,TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
	myModelInstance->SetTexture(0,2,TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));
}
Tga::RenderComponent::~RenderComponent()
{
	delete myModelInstance;
}

Tga::Component* Tga::RenderComponent::Clone(GameObject* aObj)
{
	RenderComponent* comp = new RenderComponent(aObj);
	comp->myModelInstance = new Tga::ModelInstance();

	comp->myModelInstance->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/TGE.fbx"));
	comp->myModelInstance->SetTexture(0, 0, TGE_I()->GetTextureManager().GetTexture(L"Sprites/dog.jpg"));


	return comp;
}

void Tga::RenderComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	myModelInstance->SetTransform(myGameObject->GetTransform());
}
