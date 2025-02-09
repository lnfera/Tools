#pragma once
#include <tge/graphics/Camera.h>
#include <Core/MainDrawer.h>
#include "Object/GameObject.h"
class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();
	
	void Init();
	void Update(float aDeltaTime);
	void Render();

private:
	void MoveCamera(float aDeltaTime);
	Tga::Camera myCamera;
	Tga::MainDrawer myMainDrawer;
	std::vector<Tga::GameObject*> myObjects;
};