#pragma once
#include <tge/Math/Color.h>
#include <tge/Math/Matrix4x4.h>
#include <tge/Math/Vector.h>

namespace Tga
{
class Light
{
protected:
	Color myColor;
	float myIntensity;
	Matrix4x4f myTransform;

public:

	virtual ~Light() = default;
	
	Light(const Matrix4x4f& someTransform, const Color& aColor, float aIntensity);

	float GetIntensity() const { return myIntensity; }
	Color GetColor() const { return myColor; }

	Matrix4x4f GetTransform() const { return myTransform; }
	void SetTransform(Matrix4x4f newTransform) { myTransform = newTransform; }
	void SetColor(Color newColor) { myColor = newColor; }
	void SetIntensity(float newIntensity) { myIntensity = newIntensity; }

};

} // namespace Tga