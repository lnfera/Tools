#pragma once 
#include <array>
#include <wrl/client.h>
#include <memory>
#include "../math/Vector.h"

using Microsoft::WRL::ComPtr;

struct ID3D11Buffer;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;


namespace Tga
{
	// Forward Declarations
	class DepthBuffer;
	class GBuffer;

	class GBuffer
	{
	public:
		enum class GTexture
		{
			WorldPosition,				// Wastes space and can be reconstructed from depth and screen pos, but we store it for simplicity
			Albedo,						// Straight up color
			Normal,						// stored as 0.5f + 0.5f*normal
			Material,					// RGBA: {}
			AmbientOcclusionAndCustom,	// Ambient occlusion in R, rest is unused
			Count
		};

		GBuffer();
		~GBuffer();
		static GBuffer Create(Vector2ui aSize);

		void ClearTextures();
		void SetAsActiveTarget(Tga::DepthBuffer* aDepth = nullptr);
		void SetAsResourceOnSlot(GTexture aTexture, unsigned int aSlot);
		void SetAllAsResources(unsigned aSlot);


	public:
		std::array< ComPtr<ID3D11Texture2D>, (int)GTexture::Count> myTextures;
		std::array< ComPtr<ID3D11RenderTargetView>, (int)GTexture::Count> myRTVs;
		std::array< ComPtr<ID3D11ShaderResourceView>, (int)GTexture::Count> mySRVs;
		std::shared_ptr<const D3D11_VIEWPORT> myViewport;

	};
}