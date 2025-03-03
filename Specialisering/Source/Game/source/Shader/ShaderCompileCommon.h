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

#pragma region Functions

const std::string Functions = R"(



)";

#pragma endregion

#pragma region PIXEL_SHADER
const std::string PixelStart = R"(
SamplerState defaultSampler : register(s0);

Texture2D imageText1z	: register(t1);
Texture2D imageText2z	: register(t2);
Texture2D imageText3z	: register(t3);
Texture2D imageText4z	: register(t4);
Texture2D imageText5z	: register(t5);
Texture2D imageText6z	: register(t6);
Texture2D imageText7z	: register(t7);
Texture2D imageText8z	: register(t8);
Texture2D imageText9z	: register(t9);
Texture2D imageText10z	: register(t10);
Texture2D imageText11z	: register(t11);
Texture2D imageText12z	: register(t12);
Texture2D imageText13z	: register(t13);
Texture2D imageText14z	: register(t14);
Texture2D imageText15z	: register(t15);

float4 main(ModelVertexToPixel input) : SV_TARGET
{
	float2 scaledUV = input.texCoord0;
	float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);


	float4 output = float4(1,1,1,1);
	
)";


	const std::string PixelEnd = R"(

	return output;
}
)";
#pragma endregion

} // end shader namespace
