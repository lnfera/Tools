#include "Viewport.h"
#include <string>
#include <algorithm>
//External
#include <ImGuizmo/ImGuizmo.h>	

//Engine 
#include <tge/engine.h>
#include <tge/io/Input.h>
#include <tge/graphics/DX11.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/GraphicsStateStack.h>
#include <tge/graphics/Camera.h>
#include <tge/drawers/ModelDrawer.h>
#include <tge/shaders/ModelShader.h>
#include <tge/model/ModelInstance.h>
#include <tge/model/ModelFactory.h>

//Game
#include <Scene/Scene.h>
#include <Object/GameObject.h>
#include <Object/I_AllComponents.h>
#include <Core/MainSingleton.h>
#include <Core/MainDrawer.h>

//Editor 
#include <Tools/Selection.h>
#include <Tools/Selection/CopyTool.h>
#include <Tools/ConsoleSystem.h>
#include <Tools/Commands/RemoveCommand.h>
#include <Tools/Commands/AddCommand.h>
#include <Tools/Commands/CommandManager.h>

struct IdConstantBuffer
{
	uint32_t objectId;
	uint32_t selectionId;
	uint32_t unused2;	//Padding
	uint32_t unused3;	//Padding
};

struct RenderData
{
	ComPtr<ID3D11Buffer> idConstantBuffer;
	Tga::FullscreenEffect selectionOutlineEffect;
};

namespace ViewportGlobal
{
	static int numberOfViewports = 0;
	static RenderData renderData;
	Tga::ModelShader* idShader = nullptr;
	static Tga::ModelInstance* pointLightModel = nullptr;
}
static uint32_t MouseOver(Tga::Vector2ui aPos, const Tga::RenderTarget& aTarget);

Tga::Viewport::Viewport(int aUniqueID) :
	Widget(aUniqueID)
{
	myCamera = new Tga::Camera();
	myCamera->GetTransform().SetPosition({ 0,10.f, -10.f });

	Resize({ 10,10 });

	//Create ID Buffer
	if (ViewportGlobal::numberOfViewports <= 0)
	{
		HRESULT result = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(IdConstantBuffer);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		result = DX11::Device->CreateBuffer(&bufferDesc, NULL, ViewportGlobal::renderData.idConstantBuffer.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		ViewportGlobal::idShader = new Tga::ModelShader(TGE_I());
		ViewportGlobal::idShader->Init(L"Shaders/model_shader_VS.cso", L"Shaders/Id_Shader_PS.cso");
		ViewportGlobal::renderData.selectionOutlineEffect.Init("Shaders/PostProcessSelectionOutline_PS.cso");
		ViewportGlobal::pointLightModel = new Tga::ModelInstance();
		ViewportGlobal::pointLightModel->Init(Tga::ModelFactory::GetInstance().GetModel(L"Models/Basic/SM_Sphere.fbx"));

	}

	ViewportGlobal::numberOfViewports++;
}

Tga::Viewport::~Viewport()
{
	delete myCamera;
	ViewportGlobal::numberOfViewports--;

	if (ViewportGlobal::numberOfViewports <= 0)
	{
		delete ViewportGlobal::idShader;
		delete ViewportGlobal::pointLightModel;
	}
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
		ImGui::Image((ImTextureID)myRenderTargetColor.GetShaderResourceView(), wndSize);

		if (ImGui::IsWindowHovered())
		{
			myViewportIsHovered = true;
		}
		else
		{
			myViewportIsHovered = false;
		}

		//todo fix on begin over and release

		UpdateShortcuts(aContext);

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && myViewportIsHovered)
		{
			Vector3f moveDir = 0;

			auto& trans = myCamera->GetTransform();

			Tga::Matrix4x4f mat = trans;

			if (ImGui::IsKeyDown(ImGuiKey_W))
			{
				moveDir += trans.GetForward();
			}
			if (ImGui::IsKeyDown(ImGuiKey_S))
			{
				moveDir -= trans.GetForward();
			}
			if (ImGui::IsKeyDown(ImGuiKey_D))
			{
				moveDir += trans.GetRight();
			}
			if (ImGui::IsKeyDown(ImGuiKey_A))
			{
				moveDir -= trans.GetRight();
			}

			moveDir.Normalize();

			static float moveSpeed = 10.f;

			if (ImGui::GetIO().MouseWheel != 0)
			{
				moveSpeed += ImGui::GetIO().MouseWheel * 5.f;
				moveSpeed = moveSpeed < 0.1f ? 0.1f : moveSpeed;
				moveSpeed = moveSpeed > 200.f ? 200.f : moveSpeed;
			}

			trans.SetPosition(trans.GetPosition() + (moveDir * aContext.deltaTime * moveSpeed));

			static float rotationSpeed = 0.5f;
			ImVec2 mDelta = ImGui::GetIO().MouseDelta;

			float deltaX = mDelta.y * rotationSpeed;
			float deltaY = mDelta.x * rotationSpeed;

			myCameraRotation.x += deltaX;
			myCameraRotation.y += deltaY;

			// Clamp it!
			myCameraRotation.x = std::clamp(myCameraRotation.x, -90.f, 90.f);

			trans.SetRotation(myCameraRotation);

		}
		else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)
			&& myViewportIsHovered && ImGui::IsAnyItemActive() == false && ImGuizmo::IsOver() == false)

		{
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 mousePosScreen = ImGui::GetMousePos();
			ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

			ImVec2 mousePos(mousePosScreen.x - windowPos.x, mousePosScreen.y - windowPos.y);
			mousePos.x -= windowPadding.x;
			mousePos.y -= windowPadding.y;

			ImVec2 viewport_size = ImGui::GetWindowContentRegionMax();

			Vector2ui targetSize = myRenderTargetColor.CalculateTextureSize();
			Vector2f scaleFactor = { (float)targetSize.x / viewport_size.x, (float)targetSize.y / viewport_size.y };

			if (ImGui::IsKeyReleased(ImGuiKey_LeftCtrl) == false)
			{
				Selection::ClearSelection();
			}

			if (ImGui::IsMousePosValid(&mousePos))
			{
				Vector2ui mapped_mouse = { (uint32_t)(mousePos.x * scaleFactor.x), (uint32_t)(mousePos.y * scaleFactor.y) };
				unsigned int id = MouseOver(mapped_mouse, myRenderTargetId);

				GameObject* obj = aContext.currentScene->GetObjectWithID(id);

				if (obj)
				{
					if (Selection::Contains(obj)) {
						Selection::RemoveFromSelection(obj);
					}
					else {
						Selection::AddToSelection(obj);
					}
				}
			}
		}

		myGizmo.DrawGizmos(*myCamera);
		myGizmo.DrawSettings();
	}
	ImGui::End();

	return myIsOpen;

}
void Tga::Viewport::UpdateShortcuts(EditorContext& aContext)
{
	if (!myViewportIsHovered) return;

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (ImGui::IsKeyPressed(ImGuiKey_C))
		{
			auto selection = Tga::Selection::GetSelection();

			if (selection.empty() == false)
			{
				Tga::CopyTool::SetCopyTarget(selection);
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_V))
		{
			auto selection = Tga::Selection::GetSelection();
			if (selection.empty() == false)
			{
				auto addedObjects = Tga::CopyTool::CloneCopies();
				Tga::CommandManager::DoCommand(std::make_shared<Tga::AddCommand>(aContext.currentScene, addedObjects));
				Tga::Selection::SetSelection(addedObjects);
			}
		}
	}


	if (ImGui::IsKeyPressed(ImGuiKey_Delete) &&
		ImGui::IsAnyItemActive() == false &&
		ImGui::IsWindowFocused()
		)
	{
		auto selection = Tga::Selection::GetSelection();
		if (!selection.empty())
		{
			Tga::CommandManager::DoCommand(std::make_shared<Tga::RemoveCommand>(aContext.currentScene, selection));
			selection.clear();
			Tga::Selection::ClearSelection();
		}
	}

}

void Tga::Viewport::RenderSceneToTarget(EditorContext& aContext)
{
	ID3D11ShaderResourceView* nollText = { NULL };
	Tga::DX11::Context->PSSetShaderResources(0, 1, &nollText);

	auto& engine = *Tga::Engine::GetInstance();
	auto& graphicsEngine = engine.GetGraphicsEngine();
	auto& gss = graphicsEngine.GetGraphicsStateStack();

	auto& modelDrawer = graphicsEngine.GetModelDrawer();

	gss.SetBlendState(Tga::BlendState::Disabled);
	gss.SetCamera(*myCamera);
	gss.UpdateGpuStates(true);
	gss.Push();


	DX11::Context->VSSetConstantBuffers((int)10, 1, ViewportGlobal::renderData.idConstantBuffer.GetAddressOf());
	DX11::Context->PSSetConstantBuffers((int)10, 1, ViewportGlobal::renderData.idConstantBuffer.GetAddressOf());

	// ID Target
	{
		myDepth.Clear();
		myRenderTargetId.Clear();
		myRenderTargetId.SetAsActiveTarget(&myDepth);

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		IdConstantBuffer* dataPtr;

		for (auto* obj : aContext.currentScene->GetGameObjects())
		{
			result = DX11::Context->Map(ViewportGlobal::renderData.idConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (FAILED(result))
			{
				Tga::Console::LogMessage("Error in ID Render!", Tga::Console::Colors::Red);
				return;
			}

			// Skip if clicked
			if (ImGui::IsMouseClicked(0) && Tga::Selection::Contains(obj))
			{
				continue;
			}

			//Map ID buffer
			dataPtr = (IdConstantBuffer*)mappedResource.pData;
			dataPtr->objectId = obj->GetID();
			dataPtr->selectionId = Selection::Contains(obj) ? dataPtr->objectId : 0;
			DX11::Context->Unmap(ViewportGlobal::renderData.idConstantBuffer.Get(), 0);

			if (auto* comp = obj->GetComponent<Tga::RenderComponent>())
			{
				comp->Update(aContext.deltaTime);
				modelDrawer.Draw(*comp->myModelInstance, *ViewportGlobal::idShader);
			}
			if (auto* comp = obj->GetComponent<Tga::PointLightComponent>())
			{
				float scale = comp->myPointLight->GetRange() * 0.01f;

				Matrix4x4f transform = Matrix4x4f::CreateFromScale(scale) *
					Matrix4x4f::CreateIdentityMatrix() *
					Matrix4x4f::CreateFromTranslation(comp->myPointLight->GetTransform().GetPosition());

				ViewportGlobal::pointLightModel->SetTransform(transform);

				modelDrawer.Draw(*ViewportGlobal::pointLightModel, *ViewportGlobal::idShader);
			}
		}
	}

	// Color Target
	{
		auto* mainDrawer = Tga::MainSingleton::GetInstance().GetMainDrawer();
		mainDrawer->RenderToTarget(&myRenderTargetColor, myCamera);

		//myGrid.Render(myCamera->GetTransform().GetPosition());
		myRenderTargetId.SetAsResourceOnSlot(0);
		ViewportGlobal::renderData.selectionOutlineEffect.Render();

		Tga::DX11::BackBuffer->SetAsActiveTarget(nullptr);
	}
	gss.Pop();
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

static uint32_t MouseOver(Tga::Vector2ui aPos, const Tga::RenderTarget& aTarget)
{
	Tga::Vector2ui mPos = aPos;

	ImVec2 resolution = ImGui::GetWindowSize();

	if (mPos.x < 0 || mPos.x >= resolution.x || mPos.y < 0 || mPos.y >= resolution.y)
		return 0;

	ID3D11Resource* src;
	const auto& context = Tga::DX11::Context;
	aTarget.GetShaderResourceView()->GetResource(&src);

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = 1;
	textureDesc.Height = 1;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = (DXGI_FORMAT_R32G32_UINT);
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> tmp;
	HRESULT hr = Tga::DX11::Device->CreateTexture2D(&textureDesc, nullptr, tmp.GetAddressOf());
	assert(SUCCEEDED(hr));

	D3D11_BOX srcBox;
	srcBox.left = mPos.x;
	srcBox.right = mPos.x + 1;
	srcBox.bottom = mPos.y + 1;
	srcBox.top = mPos.y;
	srcBox.front = 0;
	srcBox.back = 1;

	Tga::DX11::Context->CopySubresourceRegion(
		tmp.Get(),
		0, 0, 0, 0,
		src, 0,
		&srcBox
	);
	D3D11_MAPPED_SUBRESOURCE msr = {};
	hr = context->Map(tmp.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msr);
	assert(SUCCEEDED(hr));

	uint32_t* data = reinterpret_cast<uint32_t*>(msr.pData);

	context->Unmap(tmp.Get(), 0);

	return (data != nullptr) ? (uint32_t)*data : 0;
}