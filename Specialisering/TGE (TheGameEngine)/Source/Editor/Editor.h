#pragma once
#include <vector>
#include "EditorCommon.h"
#include <tge/math/Vector.h>

#include <tge/graphics/RenderTarget.h>
#include <tge/graphics/DepthBuffer.h>
namespace Tga
{
	class Widget;
	class Viewport;
	class AssetBank;
	class ModelInstance;
}

namespace Tga
{



	class Editor
	{
	public:
		Editor();
		~Editor();

		void Init();
		void Update(float aDeltaTime);
	private:
		//void RenderSceneToTargets();
		void RenderMenuBar(EditorContext& aContext);
		void RenderWidgets(EditorContext& aContext);
		void UpdateShortcuts(EditorContext& aContext);
		//void Resize(const Tga::Vector2ui& aSize = {0,0});
	private:
		//Widgets
		int myWidgetUniqueID = 0;
		std::vector<Widget*> myWidgets;

		//Viewport
		Scene* myScene = nullptr;
		//Camera* myCamera = nullptr;
		//RenderTarget myRenderTargetColor;
		//RenderTarget myRenderTargetId;
		//DepthBuffer myDepth;

		//SubSystems
		AssetBank* myAssetBank;
		ModelInstance* myModel = nullptr;
		Widget* myViewport = nullptr;

	};
}