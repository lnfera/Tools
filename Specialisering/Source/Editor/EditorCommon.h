#pragma once
//Foward Declaration
namespace Tga
{
	class Scene;
	class Camera;
	class RenderTarget;
	class InputManager;
}
namespace Tga
{
	struct EditorContext
	{
		float deltaTime = -1;
		Scene* currentScene = nullptr;
		InputManager* input = nullptr;
	};
}