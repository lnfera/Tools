#include <stdafx.h>
#include "MainDrawer.h"

#include <tge/model/ModelInstance.h>
#include <tge/model/ModelFactory.h>
#include <tge/shaders/ModelShader.h>

#include <tge/graphics/DX11.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/PointLight.h> 

#include <tge/drawers/ModelDrawer.h>
#include <tge/drawers/SpriteDrawer.h>

Tga::MainDrawer::MainDrawer()
{
}

Tga::MainDrawer::~MainDrawer()
{
	delete myModelToDeferedShader;
	delete myLightShader;
	delete myLightMesh;
}

bool Tga::MainDrawer::Init()
{
	bool didInit = true;

	auto& engine = *TGE_I();
	auto& graphicsEngine = engine.GetGraphicsEngine();
	myGraphicsStateStack = &graphicsEngine.GetGraphicsStateStack();
	mySpriteDrawer = &graphicsEngine.GetSpriteDrawer();
	myModelDrawer = &graphicsEngine.GetModelDrawer();

	Vector2ui intResolution = Tga::DX11::GetResolution();
	Vector2f resolution = { (float)intResolution.x, (float)intResolution.y };
	Vector2ui renderSize = engine.GetRenderSize();

	// [ GBuffer ]
	{
		myGBuffer = Tga::GBuffer::Create(renderSize);
		myDepthBuffer = Tga::DepthBuffer::Create(renderSize);
		myRenderTarget = Tga::RenderTarget::Create(renderSize, DXGI_FORMAT_R16G16B16A16_FLOAT);
		myNollSrv = { NULL };
	}

	// [ Shaders ]
	{
		myModelToDeferedShader = new Tga::ModelShader();
		didInit = myModelToDeferedShader->Init(L"shaders/PbrModelShaderVs.cso", L"shaders/model_to_gbuffer_PS.cso");
		if (!didInit)
		{
			assert(false && "Did not init Model to Defered Shader!");
		}
		myLightShader = new Tga::ModelShader();
		didInit = myLightShader->Init(L"shaders/model_shader_VS.cso", L"shaders/lights_to_fullscreen_PS.cso");
		if (!didInit)
		{
			assert(false && "Did not init Model to Light Shader!");
		}
	}

	// [ Fullscreen Effects ]
	{
		didInit = myGBufferAssembleEffect.Init("shaders/gbuffer_to_fullscreen_effect_PS.cso");
		if (!didInit)
		{
			assert(false && "Did not init GBuffer Assemble!");
		}

		didInit = myCopyEffect.Init("shaders/PostprocessCopyPS.cso");
		if (!didInit)
		{
			assert(false && "Did not init copy fullscreen effect!");
		}
	}

	// Light
	{
		myLightMesh = new Tga::ModelInstance();
		myLightMesh->Init(Tga::ModelFactory::GetInstance().GetModel(L"../EngineAssets/Models/Basic/SM_Sphere.fbx"));
	}

	return didInit;
}
void Tga::MainDrawer::AddModelToRenderGroup(ModelInstance* aInstance, ModelShader* aCustomShader)
{
	Model* model = aInstance->GetModel().get();
	for (RenderGroup& renderGroup : mySMRenderGroups)
	{
		if (renderGroup.model != model) continue;
		if (renderGroup.modelShader != aCustomShader) continue;
		renderGroup.modelInstances.push_back(aInstance);
		return;
	}
	mySMRenderGroups.push_back(RenderGroup());

	auto& instancedRenderGroup = mySMRenderGroups.back();

	instancedRenderGroup.model = model;
	instancedRenderGroup.modelShader = aCustomShader;
	instancedRenderGroup.modelInstances.push_back(aInstance);
}
void Tga::MainDrawer::RemoveModelFromRenderGroup(ModelInstance* aInstance, ModelShader* aCustomShader)
{
	Model* model = aInstance->GetModel().get();
	for (RenderGroup& renderGroup : mySMRenderGroups)
	{
		if (renderGroup.model != model) continue;
		if (renderGroup.modelShader != aCustomShader) continue;

		for (int i = 0; i < renderGroup.modelInstances.size(); i++)
		{
			if (renderGroup.modelInstances[i] == aInstance)
			{
				renderGroup.modelInstances.erase(renderGroup.modelInstances.begin() + i);
				return;
			}
		}
	}
}

void Tga::MainDrawer::AddLight(PointLight* aLight)
{
	myPointLights.push_back(aLight);
}
void Tga::MainDrawer::RemoveLight(PointLight* aLight)
{
	for (int i = 0; i < myPointLights.size(); i++)
	{
		if (myPointLights[i] == aLight) {
			myPointLights.erase(myPointLights.begin() + i);
			return;
		}
	}
}

void Tga::MainDrawer::RenderToTarget(RenderTarget* aTarget, Camera* aCamera)
{
	// Set Settings
	{
		myActiveCamera = aCamera;
	}

	// Render Pipline here
	{
		ClearBuffers();

		RenderDeferedSplice();
		RenderAssembleGBuffer();
		RenderLightPass();
		RenderForwardRendering();
		RenderPostProcessing();
		//Ui
		CopyToTarget(aTarget);
		Cleanup();
	}
}


void Tga::MainDrawer::ClearBuffers()
{
	myDepthBuffer.Clear();
	myRenderTarget.Clear(Tga::Engine::GetInstance()->GetClearColor().AsVec4());
	myGBuffer.ClearTextures();
}
void Tga::MainDrawer::RenderDeferedSplice()
{
	myGraphicsStateStack->Push();
	// [GraphicsStateStack Settings]
	{
		myGraphicsStateStack->SetCamera(*myActiveCamera);
		myGraphicsStateStack->SetBlendState(Tga::BlendState::Disabled);
		myGraphicsStateStack->SetDepthStencilState(Tga::DepthStencilState::WriteLess);
	}

	//myGraphicsStateStack->UpdateGpuStates(false);

	//Start Render to GBuffer
	myGBuffer.SetAsActiveTarget(&myDepthBuffer);

	// Loop through each item and render them
	for (auto& renderGroup : mySMRenderGroups)
	{
		auto* modelShader = renderGroup.modelShader;

		if (modelShader != nullptr && modelShader->GetRenderMode() != RenderMode::Opaque) continue;

		for (int i = 0; i < renderGroup.modelInstances.size(); i++)
		{
			if (modelShader) // Has custom shader
			{
				myModelDrawer->Draw(*renderGroup.modelInstances[i], *modelShader);
			}
			else
			{
				myModelDrawer->Draw(*renderGroup.modelInstances[i], *myModelToDeferedShader);
			}
		}
	}
	myGraphicsStateStack->Pop();
}
void Tga::MainDrawer::RenderAssembleGBuffer()
{
	myGraphicsStateStack->Push();
	// [GraphicsStateStack Settings]
	{
		myGraphicsStateStack->SetCamera(*myActiveCamera);
		myGraphicsStateStack->SetBlendState(Tga::BlendState::Disabled);
		myGraphicsStateStack->SetDepthStencilState(Tga::DepthStencilState::WriteLess);
	}

	myRenderTarget.SetAsActiveTarget(nullptr);

	myGBuffer.SetAsResourceOnSlot(Tga::GBuffer::GTexture::WorldPosition, 1);
	myGBuffer.SetAsResourceOnSlot(Tga::GBuffer::GTexture::Albedo, 2);
	myGBuffer.SetAsResourceOnSlot(Tga::GBuffer::GTexture::Normal, 3);
	myGBuffer.SetAsResourceOnSlot(Tga::GBuffer::GTexture::Material, 4);
	myGBuffer.SetAsResourceOnSlot(Tga::GBuffer::GTexture::AmbientOcclusionAndCustom, 5);

	//Render to RTV
	myGBufferAssembleEffect.Render();

	myGraphicsStateStack->Pop();
}
void Tga::MainDrawer::RenderLightPass()
{
	myGraphicsStateStack->Push();
	// [GraphicsStateStack Settings]
	{
		myGraphicsStateStack->SetCamera(*myActiveCamera);

		myGraphicsStateStack->SetBlendState(Tga::BlendState::AdditiveBlend);
		myGraphicsStateStack->SetRasterizerState(Tga::RasterizerState::FrontFaceCulling);
		myGraphicsStateStack->SetDepthStencilState(Tga::DepthStencilState::ReadOnlyGreater);

	}

	myRenderTarget.SetAsActiveTarget(nullptr);

	for (int i = 0; i < myPointLights.size(); i++)
	{
		myGraphicsStateStack->Push();
		myGraphicsStateStack->AddPointLight(*myPointLights[i]);
		myGraphicsStateStack->UpdateGpuStates(true);

		float scale = myPointLights[i]->GetRange()/* * 1.0f*/;

		Matrix4x4f transform = Matrix4x4f::CreateFromScale(scale) *
			Matrix4x4f::CreateIdentityMatrix() *
			Matrix4x4f::CreateFromTranslation(myPointLights[i]->GetTransform().GetPosition());

		myLightMesh->SetTransform(transform);

		myModelDrawer->Draw(*myLightMesh, *myLightShader);
		myGraphicsStateStack->Pop();
	}
	myGraphicsStateStack->Pop();

}
void Tga::MainDrawer::RenderForwardRendering()
{
	myGraphicsStateStack->Push();
	// [GraphicsStateStack Settings]
	{
		myGraphicsStateStack->SetCamera(*myActiveCamera);
		myGraphicsStateStack->SetBlendState(Tga::BlendState::AlphaBlend);
		myGraphicsStateStack->SetDepthStencilState(Tga::DepthStencilState::WriteLess);
		myGraphicsStateStack->SetRasterizerState(Tga::RasterizerState::BackfaceCulling);
	}
	myRenderTarget.SetAsActiveTarget(&myDepthBuffer);
	// Render SM_Objects with Transparency to forward pass
	for (auto& group : mySMRenderGroups)
	{
		auto* modelShader = group.modelShader;

		if (modelShader == nullptr || modelShader->GetRenderMode() == RenderMode::Opaque) continue;

		for (int i = 0; i < group.modelInstances.size(); i++)
		{
			myGraphicsStateStack->Push();
			int lightCount = 0;
			for (int p = 0; p < myPointLights.size(); p++)
			{
				// Distance between pivots
				Vector3f dis = myPointLights[p]->GetTransform().GetPosition() - group.modelInstances[i]->GetTransform().GetPosition();


				float maxDistance = myPointLights[p]->GetRange();

				// Used to get to sqrd for performance
				maxDistance *= maxDistance * 1.55f;

				if (dis.LengthSqr() <= maxDistance)
				{

					myGraphicsStateStack->AddPointLight(*myPointLights[p]);
					lightCount++;
				}

				if (lightCount >= 8) // change for render settings into addative
				{
					break;
				}
			}

			myModelDrawer->Draw(*group.modelInstances[i], *group.modelShader);
			myGraphicsStateStack->Pop();

		}
	}
	myGraphicsStateStack->Pop();
}
void Tga::MainDrawer::RenderPostProcessing()
{
}
void Tga::MainDrawer::CopyToTarget(RenderTarget* aTarget)
{
	aTarget->SetAsActiveTarget();

	if (myRenderPassIndex == -1)
	{
		myRenderTarget.SetAsResourceOnSlot(0);
	}
	else
	{
		myGBuffer.SetAsResourceOnSlot((Tga::GBuffer::GTexture)myRenderPassIndex, 0);
	}

	myCopyEffect.Render();
}

void Tga::MainDrawer::Cleanup()
{
	Tga::DX11::Context->PSSetShaderResources(0, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(1, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(2, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(3, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(4, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(5, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(6, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(7, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(8, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(9, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(10, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(11, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(12, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(13, 1, &myNollSrv);
	Tga::DX11::Context->PSSetShaderResources(14, 1, &myNollSrv);
}
