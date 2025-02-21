#include "ShaderOutputNodes.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>
#include <tge/texture/texture.h>
#include <tge/util/Asset.h>
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


using namespace Tga;


void Tga::RegisterShaderOutNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<ImageNode>("ShaderOut/Image", "Reads from an image");
	Tga::ScriptNodeTypeRegistry::RegisterType<ShaderOutputNode>("ShaderOut/AlbedoOnly", "Only uses albedo when rendering");
}


void Tga::ShaderOutputNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { 0.f };

	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("X");
		myXIn_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Y");
		myYIn_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Z");
		myZIn_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("W");
		myWIn_ID = aContext.FindOrCreatePin(inputPin);
	}
}
ScriptNodeResult Tga::ShaderOutputNode::Execute(ScriptExecutionContext& aContext, ScriptPinId ) const
{
	// used for finalized color when returning in pixelshader

	// Create parsing context 
	Tga::ShaderParseCompiler compiler;

	aContext.SetShaderParseCompiler(&compiler);

	//StartParsing
	//compiler.AddVariable((ShaderSource)&aContext.GetPin(myXIn_ID), aContext.ParseFromPin(myXIn_ID));
	//compiler.AddVariable((ShaderSource)&aContext.GetPin(myYIn_ID), aContext.ParseFromPin(myYIn_ID));
	//compiler.AddVariable((ShaderSource)&aContext.GetPin(myZIn_ID), aContext.ParseFromPin(myZIn_ID));
	//compiler.AddVariable((ShaderSource)&aContext.GetPin(myWIn_ID), aContext.ParseFromPin(myWIn_ID));

	std::string parsedCode = "output = float4(" +
		aContext.ParseFromPin(myXIn_ID) + "," +
		aContext.ParseFromPin(myYIn_ID) + "," + 
		aContext.ParseFromPin(myZIn_ID) + "," + 
		aContext.ParseFromPin(myWIn_ID) + ");";

	compiler.PrintParsedData();
	std::string variables = compiler.GenerateVariables();

	std::cout << variables;


	//compiler.PrintParsedData();

	std::string hlslCode =
		SHADER::BuffersHLSLI +
		SHADER::StructsHLSLI +
		SHADER::PixelStart +
		variables+
		parsedCode +
		SHADER::PixelEnd;

	//Assign Shader
	const ShaderCompileContext& compileContext = static_cast<const ShaderCompileContext&>(aContext.GetUpdateContext());

	//Set somewhere
	ID3D11PixelShader* ps = CreatePixelShaderFromString(hlslCode);

	

	if(compileContext.compilingModelShader)
	{
		compileContext.compilingModelShader->myPixelShader = ps;
	}
	else
	{
		ps->Release();
	}

	return ScriptNodeResult::Finished;
}
void Tga::ShaderOutputNode::CustomUi(float /*aSize*/)
{
	//ID3D11RenderTargetView* prevRTV = nullptr;
	//ID3D11DepthStencilView* prevDepth = nullptr;
	//Tga::DX11::Context->OMGetRenderTargets(1, &prevRTV, &prevDepth);

	//// Do image of render


	//Tga::DX11::Context->OMSetRenderTargets(1, &prevRTV, prevDepth);

	// @todo add a preview
	//ImGui::Image(myRTVSOMETINGSOMETHING, ImVec2(100.f * aSize, 100.f * aSize));
}

#pragma region ImageNode
void Tga::ImageNode::Init(const ScriptCreationContext& /*aContext*/)
{
}
ScriptNodeResult Tga::ImageNode::Execute(ScriptExecutionContext& /*aContext*/, ScriptPinId) const
{
	return ScriptNodeResult();
}
void Tga::ImageNode::CustomUi(float aSize)
{
	float size = 100.f * aSize;
	ImGui::Image((ImTextureID)myTextureResource, ImVec2(size, size));

	if (ImGui::BeginDragDropTarget())
	{
		if (auto p = ImGui::AcceptDragDropPayload("Asset"))
		{
			if (Asset* asset = reinterpret_cast<Asset*>(p->Data))
			{
				if (asset->assetType == eAssetType::eTexture)
				{
					myTextureResource = asset->assetTexture->GetShaderResourceView();
					myTexturePath = asset->relativePath.generic_string();
				}
			}
		}

		ImGui::EndDragDropTarget();
	}

}

void Tga::ImageNode::LoadFromJson(const ScriptJson& aJson)
{
	myTexturePath = aJson.json["imagePath"];

	if (myTexturePath != "")
	{
		myTextureResource = TGE_I()->GetTextureManager().GetTexture(string_cast<std::wstring>(myTexturePath).c_str())->GetShaderResourceView();
	}
}
void Tga::ImageNode::WriteToJson(ScriptJson& aJson) const
{
	aJson.json["imagePath"] = myTexturePath;
}
#pragma endregion