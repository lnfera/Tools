#pragma once
#include "../Widget.h"
#include <imgui/imgui.h>
#include <tge/math/Vector.h>

#include <tge/graphics/RenderTarget.h>
#include <tge/graphics/DepthBuffer.h>
#include <Tools/Gizmo/Gizmo.h>
#include "Tools/Gizmo/VisualGrid.h"

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
		Gizmos myGizmo;
		VisualGrid myGrid = { 16,50.f };

		Vector3f myCameraRotation = { 0,0,0 };
		Camera* myCamera = nullptr;
	};
}