#pragma once
#include <tge/math/Vector2.h>

namespace Tga
{
	enum class BlendState
	{
		Disabled,
		AlphaBlend,
		AdditiveBlend,
		Count
	};

	enum class SamplerFilter
	{
		Point,
		Bilinear,
		Trilinear,
		Count
	};

	enum class SamplerAddressMode
	{
		Clamp,
		Wrap,
		Mirror,
		Count
	};

	enum class DepthStencilState
	{
		WriteLess,
		WriteLessOrEqual,
		ReadOnlyLess,
		ReadOnlyLessOrEqual
	};

	enum class RasterizerState
	{
		BackfaceCulling,
		NoFaceCulling,
		Wireframe,
		WireframeNoCulling,
		Count,
	};

	enum ShaderMap
	{
		NORMAL_MAP,
		MAP_MAX,
	};

#pragma warning( push )
#pragma warning( disable : 26495)

#pragma warning( pop )

	struct VertexInstanced
	{
		float X, Y, Z, W;      // position
		unsigned int myVertexIndex, unused1, unused2, unused3;
	};

	struct SimpleVertex
	{
		float X, Y, Z;      // position
		float myColorR, myColorG, myColorB, myColorA;
		float myU, myV;
	};

	struct TextureRext
	{
		float myStartX;
		float myStartY;
		float myEndX;
		float myEndY;
	};
}