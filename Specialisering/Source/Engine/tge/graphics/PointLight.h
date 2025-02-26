#pragma once
#include "Light.h"

namespace Tga
{

class PointLight : public Light
{
protected:
	float myRange = 10.0f;
	
public:
	PointLight() : Light(Matrix4x4f{}, {}, {}) {};
	PointLight(const Matrix4x4f& someTransform, const Color& aColor, float aIntensity, float someRange) : Light(someTransform, aColor, aIntensity), myRange(someRange) {}
	virtual ~PointLight() = default;

	void SetRange(float someRange);
	float GetRange() const { return myRange; }
};

} // namespace Tga