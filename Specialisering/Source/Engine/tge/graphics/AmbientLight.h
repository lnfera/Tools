#pragma once
#include <wrl/client.h>
#include "Light.h"

using Microsoft::WRL::ComPtr;
struct ID3D11ShaderResourceView;

namespace Tga
{

class AmbientLight : public Light
{
	ComPtr<ID3D11ShaderResourceView> myCubemap = nullptr;
	unsigned int myNumMips = 0;
	
public:
	AmbientLight();
	AmbientLight(const Color& aColor, float aIntensity);
	AmbientLight(std::wstring& someCubemapLocation, const Color& aColor, float aIntensity);
	AmbientLight(const AmbientLight&);
	AmbientLight& operator=(const AmbientLight& other);

	virtual ~AmbientLight();

	void SetCubemap(std::wstring& someCubemapLocation);

	ID3D11ShaderResourceView* const* GetCubemap() const { return myCubemap.GetAddressOf(); }
	unsigned int GetNumMips() const { return myNumMips; }
};

} // namespace Tga
