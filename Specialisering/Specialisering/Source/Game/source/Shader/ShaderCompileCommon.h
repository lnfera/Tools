#pragma once
#include <string>

namespace SHADER
{

//const std::string CommonHLSLI = R"(
//
//)";
#pragma region Common

const std::string BuffersHLSLI = R"(
cbuffer FrameBuffer : register(b0)
{
	float4 Resolution; //myResolution.x = screen width, myResolution.y = screen height, myResolution.z = unset, myResolution.w = unset
	float4 Timings; //myTimings.x = totaltime, myTimings.y = delta time, myTimings.z = unset, myTimings.w = unset
}

cbuffer CameraBuffer : register(b1)
{
	float4x4 WorldToCamera;
	float4x4 CameraToProjection;
	float4 CameraPosition;
	float NearPlane;
	float FarPlane;
	float Unused0;
	float Unused1;
};

cbuffer LightConstantBufferData : register(b2)
{
	struct PointLightData
	{
		float4 Position;
		float4 Color;
		float Range;
		float3 garbage; // Padding, don't use arrays!
	} myPointLights[8];

	uint NumberOfLights;
	int NumEnvMapMipLevels;
	float garbage0;
	float garbage1;

	float4 AmbientLightColorAndIntensity;
	float4 DirectionalLightDirection;
	float4 DirectionalLightColorAndIntensity;
};

cbuffer ShaderSettingsConstantBuffer : register(b3)
{
	float4 customShaderParameters;
	float alphaTestThreshold;
};

cbuffer ObjectBuffer : register(b4)
{
	float4x4 ObjectToWorld;
}

cbuffer CustomShapeConstantBufferData : register(b4)
{
	float4x4 ModelToWorld;
};

cbuffer BoneBuffer : register(b5)
{
	float4x4 Bones[64];
};

)";
#pragma endregion

#pragma region Structs

const std::string StructsHLSLI = R"(
struct SpriteVertexToPixel
{
	float4 position		:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float2 tex				: TEXCOORD0;
	float4 color			: TEXCOORD2;
	float3 normal			:	NORMAL;
};

struct ModelVertexInput
{
	float4 position	    :	POSITION;
	float4 vertexColor0	:	COLOR0;
	float4 vertexColor1	:	COLOR1;
	float4 vertexColor2	:	COLOR2;
	float4 vertexColor3	:	COLOR3;
	float2 texCoord0	:	TEXCOORD0;
	float2 texCoord1	:	TEXCOORD1;
	float2 texCoord2	:	TEXCOORD2;
	float2 texCoord3	:	TEXCOORD3;
	float3 normal		:	NORMAL;
	float3 tangent		:	TANGENT;
	float3 binormal	    :	BINORMAL;
	float4 boneIndices  :   BONES;
	float4 weights      :   WEIGHTS;
};

struct ModelVertexToPixel
{
	float4 position			:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float  depth		    :	DEPTH;
	float4 vertexColor0		:	COLOR0;
	float4 vertexColor1		:	COLOR1;
	float4 vertexColor2		:	COLOR2;
	float4 vertexColor3		:	COLOR3;
	float2 texCoord0		:	TEXCOORD0;
	float2 texCoord1		:	TEXCOORD1;
	float2 texCoord2		:	TEXCOORD2;
	float2 texCoord3		:	TEXCOORD3;
	float3 normal			:	NORMAL;
	float3 tangent			:	TANGENT;
	float3 binormal			:	BINORMAL;
};

struct InstancedPixelInputType
{
	float4 position			:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float2 tex				:	TEXCOORD0;
	float4 color			:	TEXCOORD2;
	float3 normal			:	NORMAL;
	uint instanceId			:	SV_InstanceID;
};

struct PixelOutput
{
	float4 color		:	SV_TARGET;
};
)";
//const std::string PixelHLSLI = R"(
//
//)";
#pragma endregion

#pragma region PIXEL_SHADER

const std::string PixelStart = R"(
float4 main(ModelVertexToPixel input) : SV_TARGET
{
	float4 output = float4(1,1,1,1);
	
)";


	const std::string PixelEnd = R"(

	return output;
}
)";
#pragma endregion

} // end shader namespace
