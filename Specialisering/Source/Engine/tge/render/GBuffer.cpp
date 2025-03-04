#include "stdafx.h"
#include "GBuffer.h"

#include <tge/graphics/DX11.h>
Tga::GBuffer::GBuffer()
{
}

Tga::GBuffer::~GBuffer()
{
}

Tga::GBuffer Tga::GBuffer::Create(Vector2ui aSize)
{
	HRESULT result;
	std::array<DXGI_FORMAT, (int)GTexture::Count> textureFormats =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT,// Position 
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,// Albedo 
		DXGI_FORMAT_R10G10B10A2_UNORM,// Normal, 
		DXGI_FORMAT_R8G8B8A8_UNORM,// Material 
		DXGI_FORMAT_R8G8B8A8_UNORM,// AmbientOcclusionAndCustom 
	};

	GBuffer outGbuffer;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aSize.x;
	desc.Height = aSize.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	for (unsigned int idx = 0; idx < (int)GTexture::Count; idx++)
	{
		desc.Format = textureFormats[idx];
		result = Tga::DX11::Device->CreateTexture2D(&desc, nullptr, &outGbuffer.myTextures[idx]);
		assert(SUCCEEDED(result));

		result = Tga::DX11::Device->CreateRenderTargetView(
			outGbuffer.myTextures[idx].Get(),
			nullptr,
			outGbuffer.myRTVs[idx].GetAddressOf());

		assert(SUCCEEDED(result));

		result = Tga::DX11::Device->CreateShaderResourceView(
			outGbuffer.myTextures[idx].Get(),
			nullptr,
			outGbuffer.mySRVs[idx].GetAddressOf());

		assert(SUCCEEDED(result));
	}

	outGbuffer.myViewport = std::make_shared<D3D11_VIEWPORT>(
		D3D11_VIEWPORT{
		0,
		0,
		static_cast<float>(desc.Width),
		static_cast<float>(desc.Height),
		0,
		1
		});
	return outGbuffer;
}

void Tga::GBuffer::ClearTextures()
{
	Vector4f clearColor = { 0,0,0,0 };
	for (unsigned int idx = 0; idx <
		(int)GTexture::Count; idx++)
	{
		Tga::DX11::Context->ClearRenderTargetView(myRTVs[idx].Get(), &clearColor.x);
	}
}

void Tga::GBuffer::SetAsActiveTarget(Tga::DepthBuffer* aDepth)
{
	if (aDepth)
	{
		Tga::DX11::Context->OMSetRenderTargets((int)GTexture::Count, myRTVs[0].GetAddressOf(), aDepth->GetDepthStencilView());
	}
	else
	{
		Tga::DX11::Context->OMSetRenderTargets((int)GTexture::Count, myRTVs[0].GetAddressOf(), nullptr);
	}
	Tga::DX11::Context->RSSetViewports(1, myViewport.get());
}

void Tga::GBuffer::SetAsResourceOnSlot(GTexture aTexture, unsigned int aSlot)
{
	Tga::DX11::Context->PSSetShaderResources(aSlot, 1, mySRVs[(int)aTexture].GetAddressOf());
}

void Tga::GBuffer::SetAllAsResources(unsigned aSlot)
{
	Tga::DX11::Context->PSSetShaderResources(aSlot, (int)GTexture::Count, mySRVs[0].GetAddressOf());
}
