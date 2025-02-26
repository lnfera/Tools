#pragma once
#include "Light.h"

namespace Tga
{
class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light(Matrix4x4f{}, {}, {}) {};
	DirectionalLight(const Matrix4x4f& someTransform, const Color& aColor, float aIntensity) : Light(someTransform, aColor, aIntensity) {}
	virtual ~DirectionalLight() = default;
};

} // namespace Tga

