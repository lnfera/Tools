#pragma once
#include "../Widget.h"
#include <imgui/imgui.h>
#include <tge/math/Vector.h>

#include <tge/graphics/RenderTarget.h>
#include <tge/graphics/DepthBuffer.h>
namespace Tga
{
	class DepthBuffer;
	class RenderTarget;
	class Camera;
}

namespace Tga
{
	class Viewport : public Widget
	{
	public:
		Viewport(int aUniqueID);
		virtual ~Viewport();

		bool Update(EditorContext& aContext) override;

		const bool ViewportNeedsResize();
		const Vector2ui& GetViewportSize();
	private:
		void RenderSceneToTarget(EditorContext& aContext);
		void Resize(const Tga::Vector2ui& aSize);

		ImVec2 myViewportSize = {-1,-1};

		RenderTarget myRenderTargetColor;
		RenderTarget myRenderTargetId;
		DepthBuffer myDepth;

		Camera* myCamera = nullptr;
	};
}