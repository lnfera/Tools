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

#include <fstream>
using namespace Tga;


void Tga::RegisterShaderOutNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<ImageNode>("ShaderOut/Image", "Reads from an image");
	Tga::ScriptNodeTypeRegistry::RegisterType<ShaderOutputNode>("ShaderOut/AlbedoOnly", "Only uses albedo when rendering");
}


void Tga::ShaderOutputNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float4;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { Vector4f()};

	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Albedo");
		myColorIdIn_Id = aContext.FindOrCreatePin(inputPin);
	}
}
ScriptNodeResult Tga::ShaderOutputNode::Execute(ScriptExecutionContext& aContext, ScriptPinId ) const
{
	// Create parsing context 
	Tga::ShaderParseCompiler compiler;

	aContext.SetShaderParseCompiler(&compiler);

	std::string parsedCode = "output = float4(" +
		aContext.ParseFromPin(myColorIdIn_Id) + ");";

	std::string variables = compiler.GenerateVariables();

	//compiler.PrintParsedData();
	//std::cout << variables;
	//std::cout << "\n\n\n";


	//compiler.PrintParsedData();

	std::string hlslCode =
		SHADER::BuffersHLSLI +
		SHADER::StructsHLSLI +
		SHADER::PixelStart +
		variables+
		parsedCode +
		SHADER::PixelEnd;

	//std::cout << hlslCode;

	//Assign Shader
	const ShaderCompileContext& compileContext = static_cast<const ShaderCompileContext&>(aContext.GetUpdateContext());

	//Set somewhere
	ID3D11PixelShader* ps = CreatePixelShaderFromString(hlslCode);

	if(compileContext.compilingModelShader)
	{
		compileContext.compilingModelShader->myPixelShader = ps;

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
	//ID3D11RenderTargetView* prevRTV = nullptr;
	//ID3D11DepthStencilView* prevDepth = nullptr;
	//Tga::DX11::Context->OMGetRenderTargets(1, &prevRTV, &prevDepth);

	//// Do image of render


	//Tga::DX11::Context->OMSetRenderTargets(1, &prevRTV, prevDepth);

	// @todo add a preview
	//ImGui::Image(myRTVSOMETINGSOMETHING, ImVec2(100.f * aSize, 100.f * aSize));
}

#pragma region ImageNode
void Tga::ImageNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Output;

	{
		inputPin.dataType = ScriptLinkDataType::Float4;
		inputPin.defaultValue = { 0.f };

		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Col");
		myColOut_ID = aContext.FindOrCreatePin(inputPin);
	}

	inputPin.dataType = ScriptLinkDataType::Float;
	inputPin.defaultValue = { 0.f };

	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("R");
		myXOut_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("G");
		myYOut_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("B");
		myZOut_ID = aContext.FindOrCreatePin(inputPin);
	}
	{
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("A");
		myWOut_ID = aContext.FindOrCreatePin(inputPin);
	}

}
ScriptNodeResult Tga::ImageNode::Execute(ScriptExecutionContext& /*aContext*/, ScriptPinId) const
{
	return ScriptNodeResult();
}

void Tga::ImageNode::CustomUiOverlaped(float aSize)
{
	float size = 100.f * aSize;

	if (myTextureResource)
	{
		ImGui::Image((ImTextureID)myTextureResource->GetShaderResourceView(), ImVec2(size, size));
	}
	else
	{
		ImGui::Image((ImTextureID)nullptr, ImVec2(size, size));
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (auto p = ImGui::AcceptDragDropPayload("Asset"))
		{
			if (Asset* asset = reinterpret_cast<Asset*>(p->Data))
			{
				if (asset->assetType == eAssetType::eTexture)
				{
					myTextureResource = asset->assetTexture;
					myTexturePath = asset->relativePath.generic_string();
				}
			}
		}

		ImGui::EndDragDropTarget();
	}

}

ParsedData Tga::ImageNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId aID) const
{
	aContext;

	std::string imageVar = aContext.GetShaderParseCompiler()->GetOrRegisterImage((ShaderSource)this, myTextureResource);
	
	//aContext.GetShaderParseCompiler().a

	if (aID == myColOut_ID)
	{
		return ParsedData("float4", imageVar);
	}

	if (aID == myXOut_ID)
	{
		imageVar += ".x";
	}
	if (aID == myYOut_ID)
	{
		imageVar += ".y";

	}
	if (aID == myZOut_ID)
	{
		imageVar += ".z";

	}
	if (aID == myWOut_ID)
	{
		imageVar += ".w";
	}

	return ParsedData("float", imageVar);
}

void Tga::ImageNode::LoadFromJson(const ScriptJson& aJson)
{
	myTexturePath = aJson.json["imagePath"];

	if (myTexturePath != "")
	{
		myTextureResource = TGE_I()->GetTextureManager().GetTexture(string_cast<std::wstring>(myTexturePath).c_str());
	}
}
void Tga::ImageNode::WriteToJson(ScriptJson& aJson) const
{
	aJson.json["imagePath"] = myTexturePath;
}
#pragma endregion