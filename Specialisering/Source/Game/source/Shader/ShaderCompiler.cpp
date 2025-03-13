#include "ShaderCompiler.h"
#include <iostream>
#include <cassert>

#include "Shader/ShaderCompileCommon.h"
// Regular expression to match variables between @vstart and @vend

Tga::ShaderParseCompiler::ShaderParseCompiler()
{
}

Tga::ShaderParseCompiler::~ShaderParseCompiler()
{
}

void Tga::ShaderParseCompiler::PrintParsedData()
{
	std::cout << "\n";
	std::cout << "\033[32m START PRINT \033[0m\n";

	std::cout << "\033[33m Variables: \033[0m\n";

	for (auto& data : myVariables)
	{
		std::cout << /*"Source: " << data.first <<*/ " Data:" << data << '\n';
	}

	std::cout << "\033[33m Connections: \033[0m\n";

	std::cout << "\033[33m Registered Variables : \033[0m\n";

	for (auto& data : myRegisteredVariables)
	{
		std::cout << "Source: " << data.first << " Variable: " << data.second << '\n';
	}


	std::cout << "\033[31m END PRINT \033[0m\n";

	std::cout << "\n";
}

void Tga::ShaderParseCompiler::RegisterVariable(const ShaderSource aSource, const std::string& aDataType, const std::string& aValue)
{
	// should be giving something like " float var2 0.3333f
	std::string uniqueVarName = "var" + std::to_string(++myUniqueVariableId) + "z";
	std::string reg = aDataType + " " + uniqueVarName + " " + aValue;

	myVariables.push_back(reg);
	myRegisteredVariables[aSource] = reg;
	myRegisteredVariablesNames[aSource] = uniqueVarName;
}

bool Tga::ShaderParseCompiler::isVariableRegistered(const ShaderSource aSource)
{
	if (myRegisteredVariables.find(aSource) != myRegisteredVariables.end())
	{
		return true;
	}
	return false;
}

std::string Tga::ShaderParseCompiler::GetRegisteredVariableName(const ShaderSource aSource)
{
	return myRegisteredVariablesNames[aSource];
}

std::string Tga::ShaderParseCompiler::GetOrRegisterTexture(const ShaderSource aSource, int aSlot, std::string aUV)
{
	if (myRegisteredVariables.find(aSource) == myRegisteredVariables.end())
	{

		// create unique name
		std::string uniqueVarName = "image" + std::to_string(aSlot) + "z";

		// Check for already existing 
		for (auto& entry : myRegisteredVariablesNames)
		{
			if (entry.second.find(uniqueVarName) != -1)
			{
				std::string varName = GetUniqueVariableName();
				std::string reg = "float4 " + varName + " imageText" + std::to_string(aSlot) + "z.Sample(defaultSampler, " + aUV + ".xy).rgba";

				//Referense old registry
				myVariables.push_back(reg);
				myRegisteredVariables[aSource] = reg;
				myRegisteredVariablesNames[aSource] = varName;
				return myRegisteredVariablesNames[aSource];
			}
		}

		//Create new registry
		std::string reg = "float4 " + uniqueVarName + " imageText" + std::to_string(aSlot) + "z.Sample(defaultSampler, " + aUV + ".xy).rgba";

		myVariables.push_back(reg);
		myRegisteredVariables[aSource] = reg;
		myRegisteredVariablesNames[aSource] = uniqueVarName;
	}
	return myRegisteredVariablesNames[aSource];
}

std::string Tga::ShaderParseCompiler::GetOrRegisterImage(const ShaderSource aSource, TextureResource* aTexture, std::string aUV)
{
	if (myRegisteredVariables.find(aSource) == myRegisteredVariables.end())
	{
		int imgId = myUniqueImageId++;
		std::string uniqueVarName = "image" + std::to_string(imgId) + "z";
		std::string reg = "float4 " + uniqueVarName + " imageText" + std::to_string(imgId) + "z.Sample(defaultSampler, " + aUV + ".xy).rgba";

		myVariables.push_back(reg);
		myRegisteredVariables[aSource] = reg;
		myRegisteredVariablesNames[aSource] = uniqueVarName;
		myImages.push_back(aTexture);
		return uniqueVarName;

	}
	return myRegisteredVariablesNames[aSource];
}

std::string Tga::ShaderParseCompiler::GenerateVariables()
{
	std::string output;

	for (int i = 0; i < myVariables.size(); i++)
	{
		int itr = (int)myVariables[i].find_first_of("z") + 1;

		std::string fixedVariable = myVariables[i];
		fixedVariable.insert(fixedVariable.begin() + itr, '=');

		output += fixedVariable + ";\n";
	}

	return output;
}

void  Tga::ShaderParseCompiler::GeneratePBRShader(std::string& aHlslOut, const std::string& aAlbedoResult, const std::string& aNormalResult, const std::string& aMaterialResult, const std::string& aFXResult)
{

	std::string fixedNormal = aNormalResult;
	if (fixedNormal == "float4(-1.000000f,-1.000000f,-1.000000f,-1.000000f)")
	{
		fixedNormal = "float4(input.normal.xyz,1)";
	}

	std::string variables = GenerateVariables();
	std::string parsedResult =
		"albedo = " + aAlbedoResult + ";\n" +
		"normal = " + fixedNormal + ";\n" +
		"material = " + aMaterialResult + ";\n" +
		"fx = " + aFXResult + ";\n";

	// [Start Generating HLSL]
	aHlslOut.clear();

	// Load Common Data
	aHlslOut = SHADER::BuffersHLSLI +
		SHADER::StructsHLSLI +
		SHADER::ImagesAndSampler +
		SHADER::Functions;

	// [Start of Main Shader]
	{
		aHlslOut += R"( 
	PixelOutput main(ModelVertexToPixel input)
    {
	    PixelOutput result;
        float2 scaledUV = input.texCoord0;
	    float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);

        float4 albedo   = float4(0,0,0,0);
	    float4 normal   = float4(0,0,0,0);
	    float4 material = float4(0,0,0,0);
        float4 fx       = float4(0,0,0,0);

        // Variables
	)";
	}
	aHlslOut += variables;

	aHlslOut += "\n\n //Output values\n";

	aHlslOut += parsedResult;

	// Normal
	{
		// Check for fixed input normal or pixel normal
		if (fixedNormal == aNormalResult)
		{
			aHlslOut += R"(

		normal.xy = 2.0f * normal.xy - 1.0f;
		normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
		normal = normalize(normal);

		float3x3 TBN = float3x3(
			normalize(input.tangent.xyz),
			normalize(-input.binormal.xyz),
			normalize(input.normal.xyz)
		);

		// Can save an instruction here by instead doing
		// normalize(mul(normal, TBN)); It works because
		// TBN is a 3x3 and therefore TBN^T is the same
		// as TBN^-1. However, it is considered good form
		// to do this.
		TBN = transpose(TBN);
		float3 pixelNormal = normalize(mul(TBN, normal));
)";
		}
		else
		{
			aHlslOut += R"(

		float3 pixelNormal = normal.xyz;
)";
		}
	}

	// Shader End
	{
		aHlslOut += R"(
		  // TGA Channel Pack. ORM.
	    // Metalness, Roughness, Emissive, Emissive Strength (opt).

        float ambientOcclusion = material.r;
        float metalness = material.b;
        float roughness = material.g;

        float emissive = fx.r;

	    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
	    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);

	    float3 ambiance = EvaluateAmbiance(
	    	environmentTexture, pixelNormal, input.normal.xyz,
	    	toEye, roughness,
	    	ambientOcclusion, diffuseColor, specularColor
	    );

	    float3 directionalLight = EvaluateDirectionalLight(
	    	diffuseColor, specularColor, pixelNormal, roughness,
	    	DirectionalLightColorAndIntensity.xyz, DirectionalLightDirection.xyz, toEye.xyz
	    );

	    float3 pointLights = 0; // <- The sum of all point lights.
	    for(unsigned int p = 0; p < NumberOfLights; p++)
	    {
	    	pointLights += EvaluatePointLight(
	    		diffuseColor, specularColor, pixelNormal, roughness,
	    		myPointLights[p].Color.rgb, myPointLights[p].Color.w, myPointLights[p].Range, myPointLights[p].Position.xyz,
	    		toEye.xyz, input.worldPosition.xyz);
	    }
	    
	    float3 emissiveAlbedo = albedo.rgb * emissive;
	    float3 radiance = ambiance + directionalLight + pointLights + emissiveAlbedo;

        result.color.rgb = (float3) radiance;
	    result.color.a = albedo.a;
	    return result;
		}

		)";

	}
}

void Tga::ShaderParseCompiler::GenerateDeferredShader(std::string& aHlslOut, const std::string& aAlbedoResult, const std::string& aNormalResult, const std::string& aMaterialResult, const std::string& aFXResult)
{
	std::string fixedNormal = aNormalResult;
	if (fixedNormal == "float4(-1.000000f,-1.000000f,-1.000000f,-1.000000f)")
	{
		fixedNormal = "float4(input.normal.xyz,1)";
	}

	std::string variables = GenerateVariables();
	std::string parsedResult =
		"albedo = " + aAlbedoResult + ";\n" +
		"normal = " + fixedNormal + ";\n" +
		"material = " + aMaterialResult + ";\n" +
		"fx = " + aFXResult + ";\n";


	// [Start Generating HLSL]
	aHlslOut.clear();

	// Load Common Data
	aHlslOut = SHADER::BuffersHLSLI +
		SHADER::StructsHLSLI +
		SHADER::ImagesAndSampler +
		SHADER::Functions;

	// [Start of Deferred Shader]
	{
		aHlslOut += R"( 
struct GBufferOutput
{
    float4 myWorldPosition				: SV_TARGET0;
    float4 myAlbedo						: SV_TARGET1;
    float4 myNormal						: SV_TARGET2;
    float4 myMaterial					: SV_TARGET3;
    float4 myAmbientOcclusionAndCustom	: SV_TARGET4;
};

GBufferOutput main(ModelVertexToPixel input)
{
    float2 scaledUV = input.texCoord0;
	float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);

	float4 albedo = float4(0,0,0,0);
	float4 normal = float4(0,0,0,0);
	float4 material = float4(0,0,0,0);
    float4 fx = float4(0,0,0,0);

	float ambientOcclusion = input.normal.y;
	
	GBufferOutput output;


		)";
	}

	aHlslOut += "	//Variables \n" + variables;
	aHlslOut += "\n\n	//ParsedResult \n" + parsedResult;

	// End of Deferred
	{
		if (fixedNormal == aNormalResult)
		{
			aHlslOut += R"(
			normal.xy = 2.0f * normal.xy - 1.0f;
			normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
			normal = normalize(normal);

			float3x3 TBN = float3x3(
				normalize(input.tangent.xyz),
				normalize(-input.binormal.xyz),
				normalize(input.normal.xyz)
			);
			TBN = transpose(TBN);

			float3 pixelNormal = normalize(mul(TBN, normal.xyz));
			)";

		}
		else
		{
			aHlslOut += R"(
			float3 pixelNormal = normal.xyz;
			)";
		}

		aHlslOut += R"(
	output.myWorldPosition = input.worldPosition;
    output.myAlbedo = albedo;
    output.myNormal = float4(0.5f + 0.5f * pixelNormal, 1.0f);
    output.myMaterial = material;
    output.myAmbientOcclusionAndCustom = float4(ambientOcclusion, /*Emission*/fx.r, 1.f, 1.f); // gba are unused, put whatever data you want here!
    
    if (albedo.w <= alphaTestThreshold)
    {
        discard;
        output.myAlbedo = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }

	return output;
	}
	)";
	}
}

std::string Tga::ShaderParseCompiler::GetUniqueVariableName()
{
	return "var" + std::to_string(++myUniqueVariableId) + "z";
}
