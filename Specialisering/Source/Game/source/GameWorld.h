#pragma once
#include <tge/graphics/Camera.h>
#include <Core/MainDrawer.h>
#include "Object/GameObject.h"
#include "Scene/Scene.h"

#include <memory>
#include "Scripting/ScriptRuntimeInstance.h"
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
	Tga::Scene* myScene = nullptr;

	std::unique_ptr<Tga::ScriptRuntimeInstance> myScriptInstance;
};