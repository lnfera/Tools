#pragma once
//#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <iostream>
#include <tge/graphics/DX11.h>
#include <fstream>
static inline ID3DBlob* CompileShaderFromString(const std::string& aShaderCode, const std::string& aEntryPoint, const std::string& aTarget)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompile(
		aShaderCode.c_str(),
		aShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		aEntryPoint.c_str(),
		aTarget.c_str(),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shaderBlob,
		&errorBlob
	);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			std::cerr << "Shader compilation error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			errorBlob->Release();
		}
		return nullptr;
	}

	if (errorBlob) errorBlob->Release();

	return shaderBlob;

}

static inline ID3D11PixelShader* CreatePixelShader(ID3D11Device* aDevice, ID3DBlob* aShaderBlob)
{
	ID3D11PixelShader* pixelShader = nullptr;
	HRESULT hr = aDevice->CreatePixelShader(
		aShaderBlob->GetBufferPointer(),
		aShaderBlob->GetBufferSize(),
		nullptr,
		&pixelShader
	);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create pixel shader." << std::endl;
		return nullptr;
	}

	return pixelShader;
}
static inline ID3D11PixelShader* CreatePixelShaderFromString(const std::string& aShaderCode, const std::string& aEntryPoint = "main", const std::string& aTarget = "ps_5_0")
{
	ID3D11Device* device = Tga::DX11::Device;

	// Compile the HLSL code
	ID3DBlob* hlslBlob = CompileShaderFromString(aShaderCode, aEntryPoint, aTarget);
	if (hlslBlob == nullptr)
	{
		std::ofstream file("shader_crash_logs.log");
		std::clog.rdbuf(file.rdbuf());
		file << aShaderCode;
		file.close();
		std::cout;
		

		assert(hlslBlob != nullptr && "HLSL CODE DID NOT COMPILE!");
	}

	// Create the pixel shader
	ID3D11PixelShader* pixelShader = CreatePixelShader(device, hlslBlob);
	hlslBlob->Release();
	if (pixelShader == nullptr)
	{
		assert(pixelShader != nullptr && "PIXELSHADER DID NOT COMPILE!");
	}

	return pixelShader;
}