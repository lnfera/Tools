#pragma once
#include <string>
#include <vector>
#include <tge/render/GBuffer.h>
#include <tge/render/RenderGroup.h>
#include <tge/graphics/DepthBuffer.h>
#include <tge/graphics/RenderTarget.h>
#include <tge/graphics/FullscreenEffect.h>
namespace Tga
{
	class Camera;

	class RenderTarget;
	class ModelDrawer;
	class SpriteDrawer;
	class GraphicsStateStack;

	class PointLight;
	class ModelInstance;
}

namespace Tga
{
	class MainDrawer
	{
	public:
		MainDrawer();
		~MainDrawer();

		bool Init();

		void AddModelToRenderGroup(ModelInstance* aInstance, ModelShader* aCustomShader = nullptr);
		void RemoveModelFromRenderGroup(ModelInstance* aInstance, ModelShader* aCustomShader = nullptr);
		
		void AddLight(PointLight* aLight);
		void RemoveLight(PointLight* aLight);

		void RenderToTarget(RenderTarget* aTarget, Camera* aCamera);
		void CyclePassIndex() 
		{ 
			myRenderPassIndex = ++myRenderPassIndex < (int)GBuffer::GTexture::Count ? myRenderPassIndex : -1;
		}

		DepthBuffer* GetDepthBuffer() { return &myDepthBuffer; };
		
	private:
		// Render Pipeline
		void ClearBuffers();
		void RenderDeferedSplice();
		void RenderAssembleGBuffer();
		void RenderLightPass();
		void RenderForwardRendering();

		void RenderPostProcessing();
		void CopyToTarget(RenderTarget* aTarget);
		void Cleanup();

	private:
		int myRenderPassIndex = -1;

		// Drawers + Settings
		Tga::GraphicsStateStack* myGraphicsStateStack = nullptr;
		Tga::ModelDrawer* myModelDrawer = nullptr;
		Tga::SpriteDrawer* mySpriteDrawer = nullptr;

		// Defered
		Tga::GBuffer myGBuffer;
		Tga::DepthBuffer myDepthBuffer;
		ID3D11ShaderResourceView* myNollSrv;

		//Cameras
		Camera* myActiveCamera = nullptr;

		// RenderTargets
		Tga::RenderTarget myRenderTarget;

		// RenderObjects
		std::vector<RenderGroup> mySMRenderGroups;

		// Shaders
		ModelShader* myModelToDeferedShader = nullptr;

		// FullScreen Effects
		FullscreenEffect myCopyEffect;
		FullscreenEffect myGBufferAssembleEffect;
		FullscreenEffect myLightRenderEffect;

		//Lights
		std::vector<PointLight*> myPointLights;
	};
}
