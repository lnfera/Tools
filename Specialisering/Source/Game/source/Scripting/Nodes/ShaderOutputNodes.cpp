#include "ShaderOutputNodes.h"

#include <tge/engine.h>
#include <tge/shaders/ModelShader.h>

#include <Shader/DXShader.h>
#include <Shader/ShaderCompileCommon.h>
#include <Shader/ShaderManager.h>
#include <Shader/ShaderCompiler.h>

#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/Context/ScriptUpdateContext.h>
#include <Scripting/ScriptJson.h>

#include <Core/MainSingleton.h>

#include <fstream>
using namespace Tga;


void Tga::RegisterShaderOutNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<ShaderOutputNode>("ShaderOut/AlbedoOnly", "Only uses albedo when rendering");
	Tga::ScriptNodeTypeRegistry::RegisterType<PBROutputNode>("ShaderOut/PBROutput", "Splits the images into the defered rednering pipeline");
}

#pragma region ShaderOut
void Tga::ShaderOutputNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float4;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { Vector4f() };

	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Albedo");
		myColorIdIn_Id = aContext.FindOrCreatePin(inputPin);
	}
}
ScriptNodeResult Tga::ShaderOutputNode::Execute(ScriptExecutionContext& aContext, ScriptPinId) const
{
	// Create parsing context 
	Tga::ShaderParseCompiler compiler;

	aContext.SetShaderParseCompiler(&compiler);

	std::string parsedCode = "output = float4(" +
		aContext.ParseFromPin(myColorIdIn_Id) + ");";

	std::string variables = compiler.GenerateVariables();

	std::string hlslCode =
		SHADER::BuffersHLSLI +
		SHADER::ImagesAndSampler +
		SHADER::StructsHLSLI +
		SHADER::PixelStart +
		variables +
		parsedCode +
		SHADER::ShaderEnd;

	//std::cout << hlslCode;

	//Assign Shader
	const ShaderCompileContext& compileContext = static_cast<const ShaderCompileContext&>(aContext.GetUpdateContext());

	//Set somewhere
	ID3D11PixelShader* ps = CreatePixelShaderFromString(hlslCode);

	if (compileContext.compilingModelShader)
	{
		compileContext.compilingModelShader->myPixelShader = ps;
		*compileContext.hlslCode = hlslCode;

		auto images = compiler.GetImages();
		compileContext.compilingModelShader->ResetTextureResource();
		for (int i = 0; i < images.size(); i++)
		{
			compileContext.compilingModelShader->SetTextureResource(images[i], i);
		}
	}
	else
	{
		ps->Release();
	}

	return ScriptNodeResult::Finished;
}
void Tga::ShaderOutputNode::CustomUiBelow(float /*aSize*/)
{
}

void Tga::PBROutputNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float4;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { Vector4f() };

	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Albedo");
		myAlbedoIn_Id = aContext.FindOrCreatePin(inputPin);
	
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Normal");
		myNormalIn_Id = aContext.FindOrCreatePin(inputPin);

		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Material");
		myMaterialIn_Id = aContext.FindOrCreatePin(inputPin);

		inputPin.name = ScriptStringRegistry::RegisterOrGetString("FX");
		myFxIn_Id = aContext.FindOrCreatePin(inputPin);
	}
}
ScriptNodeResult Tga::PBROutputNode::Execute(ScriptExecutionContext& aContext, ScriptPinId) const
{
	// Create parsing context 
	Tga::ShaderParseCompiler compiler;
	const ShaderCompileContext& compileContext = static_cast<const ShaderCompileContext&>(aContext.GetUpdateContext());

	aContext.SetShaderParseCompiler(&compiler);

	std::string parsedCode = 
		"albedo = " + aContext.ParseFromPin(myAlbedoIn_Id) + ";\n" +
		"normal = " + aContext.ParseFromPin(myNormalIn_Id) + ";\n" +
		"material = " + aContext.ParseFromPin(myMaterialIn_Id) + ";\n" +
		"fx = " + aContext.ParseFromPin(myFxIn_Id) + ";\n";

	std::string variables = compiler.GenerateVariables();

	std::string hlslCode = "";

	switch (myRenderMode)
	{
	case Tga::RenderMode::Transparent:	// Render to forwardRendering
		compileContext.compilingModelShader->SetRenderMode(RenderMode::Transparent);

		hlslCode =
			SHADER::BuffersHLSLI +
			SHADER::StructsHLSLI +
			SHADER::ImagesAndSampler +
			SHADER::Functions +
			SHADER::PBRStart +
			variables +
			parsedCode +
			SHADER::PBREnd;
		break;
	case Tga::RenderMode::Opaque:
	default:		// Render to Deferred Rendering !
		compileContext.compilingModelShader->SetRenderMode(RenderMode::Opaque);

	hlslCode =
		SHADER::BuffersHLSLI +
		SHADER::StructsHLSLI +
		SHADER::ImagesAndSampler + 
		SHADER::DeferredStructHLSLI +
		SHADER::Functions +
		SHADER::START_DEFERRED +
		variables +
		parsedCode +
		SHADER::END_DEFERRED;
		break;
	}


	//Assign Shader
	//Set somewhere
	ID3D11PixelShader* ps = CreatePixelShaderFromString(hlslCode);
	if (compileContext.compilingModelShader)
	{
		compileContext.compilingModelShader->myPixelShader = ps;
		*compileContext.hlslCode = hlslCode;

		auto images = compiler.GetImages();
		compileContext.compilingModelShader->ResetTextureResource();
		for (int i = 0; i < images.size(); i++)
		{
			compileContext.compilingModelShader->SetTextureResource(images[i], i);
		}
	}
	else
	{
		ps->Release();
	}

	return ScriptNodeResult::Finished;
}

void Tga::PBROutputNode::CustomUiBelow(float aSizeMod)
{
	aSizeMod; 
	
	const char* renderItems[(int)RenderMode::COUNT][15] = {
		"Defered - Opaque",
		"Forward - Transparency"
	};

	int currentRenderMode = (int)myRenderMode;
	ImGui::Text("RenderMode");
	ImGui::SetNextItemWidth(70 * aSizeMod);
	if(ImGui::Combo("##RenderMode", &currentRenderMode, renderItems[0], IM_ARRAYSIZE(renderItems)))
	{
		myRenderMode = (RenderMode)currentRenderMode;
	}
}
void Tga::PBROutputNode::LoadFromJson(const ScriptJson& aJsonFile)
{
	if (aJsonFile.json.contains("RenderMode"))
	{
		myRenderMode = (RenderMode)aJsonFile.json["RenderMode"];
	}
}
void Tga::PBROutputNode::WriteToJson(ScriptJson& aJsonFile) const
{
	aJsonFile.json["RenderMode"] = (int)myRenderMode;
}

#pragma endregion
