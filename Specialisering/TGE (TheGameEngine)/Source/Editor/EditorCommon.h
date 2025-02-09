#pragma once
//Foward Declaration
namespace Tga
{
	class Scene;
	class Camera;
	class RenderTarget;
}
namespace Tga
{
	struct EditorContext
	{
		float deltaTime = -1;
		Scene* currentScene = nullptr;
	};
}