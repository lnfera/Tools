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
		void RenderMenuBar(EditorContext& aContext);
		void RenderWidgets(EditorContext& aContext);
		void UpdateShortcuts(EditorContext& aContext);
	private:
		//Widgets
		int myWidgetUniqueID = 0;
		std::vector<Widget*> myWidgets;

		//Scene
		Scene* myScene = nullptr;

		//SubSystems
		AssetBank* myAssetBank;
		ModelInstance* myModel = nullptr;

	};
}