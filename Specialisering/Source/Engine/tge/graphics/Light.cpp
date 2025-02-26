#include "stdafx.h"
#include "Light.h"

using namespace Tga;

Light::Light(const Matrix4x4f& someTransform, const Color& aColor, float aIntensity)
{
	myTransform = someTransform;
	myColor = aColor;
	myIntensity = aIntensity;
}
