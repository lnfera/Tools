#include "Viewport.h"
#include <string>

#include <tge/engine.h>
#include <tge/io/Input.h>
#include <tge/graphics/DX11.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/GraphicsStateStack.h>
#include <tge/graphics/Camera.h>
#include <tge/drawers/ModelDrawer.h>

#include <Scene/Scene.h>
#include <Object/GameObject.h>
#include <Object/I_AllComponents.h>

Tga::Viewport::Viewport(int aUniqueID) :
	Widget(aUniqueID)
{
	myCamera = new Tga::Camera();

	Resize({ 0,0 });
}

Tga::Viewport::~Viewport()
{
	delete myCamera;
}

bool Tga::Viewport::Update(EditorContext& aContext)
{
	aContext;
	std::string lable = "Viewport##" + std::to_string(myID);
	if (ImGui::Begin(lable.c_str(), &myIsOpen))
	{
		ImVec2 wndSize = ImGui::GetContentRegionAvail();

		// Check if the viewport size has changed
		if (myViewportSize.x != wndSize.x || myViewportSize.y != wndSize.y)
		{
			myViewportSize = wndSize;
			Resize({ (uint32_t)myViewportSize.x, (uint32_t)myViewportSize.y });
		}
		// Render the scene to the resized render target
		RenderSceneToTarget(aContext);

		// Display the render target in ImGui
		ImGui::Image(myRenderTargetColor.GetShaderResourceView(), wndSize);

		auto& io = Tga::Engine::GetInstance()->GetInputManager();
		if (io.IsKeyDown(VK_RBUTTON) && ImGui::IsWindowHovered())
		{
			Vector3f moveDir = 0;

			auto& trans = myCamera->GetTransform();
			//auto& trans = aContext.camera->GetTransform();

			Tga::Matrix4x4f mat = trans.GetMatrix();

			if (io.IsKeyDown('W'))
			{
				moveDir += {mat(3, 1), mat(3, 2), mat(3, 3)};
			}
			if (io.IsKeyDown('S'))
			{
				moveDir -= {mat(3, 1), mat(3, 2), mat(3, 3)};

			}
			if (io.IsKeyDown('D'))
			{
				moveDir += {mat(1, 1), mat(1, 2), mat(1, 3)};
			}
			if (io.IsKeyDown('A'))
			{
				moveDir -= {mat(1, 1), mat(1, 2), mat(1, 3)};
			}

			moveDir.Normalize();

			float moveSpeed = 10.f;
			trans.SetPosition(trans.GetPosition() + (moveDir * aContext.deltaTime * moveSpeed));

			Vector2f mDelta = io.GetMouseDelta();
			trans.AddRotation({ 0,1 * aContext.deltaTime * mDelta.x * 30.f,0 });
			trans.AddRotation({ 1 * aContext.deltaTime * mDelta.y * 30.f,0,0 });
		}
	}
	ImGui::End();

	return myIsOpen;

}

void Tga::Viewport::RenderSceneToTarget(EditorContext& aContext)
{
	auto& engine = *Tga::Engine::GetInstance();
	auto& graphicsEngine = engine.GetGraphicsEngine();
	auto& gss = graphicsEngine.GetGraphicsStateStack();

	auto& modelDrawer = graphicsEngine.GetModelDrawer();

	myRenderTargetId.Clear();
	myRenderTargetColor.Clear(engine.GetClearColor().AsVec4());

	gss.SetCamera(*myCamera);
	gss.UpdateGpuStates();

	// ID Target
	{

	}

	// Color Target
	{
		myRenderTargetColor.SetAsActiveTarget(&myDepth);

		for (auto* obj : aContext.currentScene->GetGameObjects())
		{
			if (auto* comp = obj->GetComponent<Tga::RenderComponent>())
			{
				comp->Update(0.1f);
				modelDrawer.Draw(*comp->myModelInstance);
			}
		}

		Tga::DX11::BackBuffer->SetAsActiveTarget(nullptr);
	}
}

void Tga::Viewport::Resize(const Tga::Vector2ui& aSize)
{
	Vector2ui resolution = aSize;
	if (resolution.x == 0 || resolution.y == 0)
	{
		resolution = Tga::Engine::GetInstance()->GetRenderSize();
	}

	Vector2f center = { (float)resolution.x * 0.5f, (float)resolution.y * 0.5f };

	myRenderTargetColor = RenderTarget::Create(resolution, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	myRenderTargetId = RenderTarget::Create(resolution, DXGI_FORMAT_R32G32_UINT);
	myDepth = DepthBuffer::Create(resolution);


	myCamera->SetPerspectiveProjection(
		90,						//FOV
		{
			(float)resolution.x,// Resolution X
			(float)resolution.y	// Resolution Y
		},
		0.1f,					//Nearplane
		50000.0f				//FarPlane
	);
}