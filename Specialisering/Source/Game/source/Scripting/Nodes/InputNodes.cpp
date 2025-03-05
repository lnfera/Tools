#include "InputNodes.h"

#include <tge/texture/texture.h>
#include <tge/util/Asset.h>
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>

#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptJson.h>

using namespace Tga;
void Tga::RegisterInputNodes()
{
	//Tga::ScriptNodeTypeRegistry::RegisterType<FresnelNode>("Input/Fresnel", "Creates values using the viewing direction of the object");
	Tga::ScriptNodeTypeRegistry::RegisterType<PixelInputNode>("Input/PixelInput", "Reads the input data from the pixel");
	Tga::ScriptNodeTypeRegistry::RegisterType<TextureCoordinateNode>("Input/TextureCoords", "Reads uv values");
	Tga::ScriptNodeTypeRegistry::RegisterType<ImageNode>("Input/Image", "Reads Values from an image");
}

#pragma region FresnelNode
//Tga::FresnelNode::FresnelNode()
//{
//}
//
//Tga::FresnelNode::~FresnelNode()
//{
//}
//
//void Tga::FresnelNode::Init(const Tga::ScriptCreationContext& aContext)
//{
//	{
//		ScriptPin outputPin = {};
//		outputPin.dataType = ScriptLinkDataType::Float;
//		outputPin.node = aContext.GetNodeId();
//		outputPin.role = ScriptPinRole::Input;
//		outputPin.defaultValue = { 0.f };
//		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("");
//
//		aContext.FindOrCreatePin(outputPin);
//	}
//}
//
//ParsedData Tga::FresnelNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
//{
//
//
//	return ParsedData();
//}

#pragma endregion

void Tga::PixelInputNode::Init(const Tga::ScriptCreationContext& aContext)
{
	// Out 4
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float4;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;

		// Position
		{
			outputPin.defaultValue = { Vector4f() };
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Position");

			myPositionOut_Id = aContext.FindOrCreatePin(outputPin);
		}
		// World Position
		{
			outputPin.defaultValue = { Vector4f() };
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("World Position");

			myWorldPositionOut_Id = aContext.FindOrCreatePin(outputPin);
		}
		// Normal 
		{
			outputPin.defaultValue = { Vector4f() };
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Normal");

			myNormalOut_Id = aContext.FindOrCreatePin(outputPin);
		}
	}

	// Out float
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.0f };
		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Depth");
		myDepthOut_Id = aContext.FindOrCreatePin(outputPin);
	}
}
ParsedData Tga::PixelInputNode::ParseInputPin(Tga::ScriptExecutionContext& /*aContext*/, ScriptPinId aPin) const
{

	if (aPin == myDepthOut_Id)
	{
		return ParsedData("float", "input.depth");
	}
	if (aPin == myPositionOut_Id)
	{
		return ParsedData("float4", "input.position");
	}
	if (aPin == myWorldPositionOut_Id)
	{
		return ParsedData("float4", "input.worldPosition");
	}
	if (aPin == myNormalOut_Id)
	{
		return ParsedData("float4", "input.normal.xyzz");
	}

	return ParsedData();
}

// Textcoords
void Tga::TextureCoordinateNode::Init(const Tga::ScriptCreationContext& aContext)
{
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float2;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { Vector2f()};

		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("UV");
		myScaledUVOutput = aContext.FindOrCreatePin(outputPin);

		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("ScreenPos");
		myScreenPosOutput = aContext.FindOrCreatePin(outputPin);
	}
}

ScriptLinkData Tga::TextureCoordinateNode::ReadPin(Tga::ScriptExecutionContext&, ScriptPinId aPin) const
{
	aPin;
	return { Vector2f() };
}

ParsedData Tga::TextureCoordinateNode::ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId aPin) const
{
	if (aPin == myScaledUVOutput)
	{
		return { "float2", "scaledUV"};
	}
	if (aPin == myScreenPosOutput)
	{
		return { "float2", "float2(input.position.xy / Resolution.xy)"};
	}

	return ParsedData();
}

#pragma region ImageNode
void Tga::ImageNode::Init(const ScriptCreationContext& aContext)
{
	// Input
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

	// Output
	{

		ScriptPin inputPin = {};
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Input;

		inputPin.dataType = ScriptLinkDataType::Float2;
		inputPin.defaultValue = { Vector2f(-1,-1) };

		inputPin.name = ScriptStringRegistry::RegisterOrGetString("UV");
		myUVIn_ID = aContext.FindOrCreatePin(inputPin);
	}

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

ScriptLinkData Tga::ImageNode::ReadPin(Tga::ScriptExecutionContext&, ScriptPinId aPin) const
{
	if (aPin == myColOut_ID)
	{
		return { Vector4f() };
	}

	return { 0.f };
}
ParsedData Tga::ImageNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId aID) const
{
	aContext;
	aID;
	Vector2f uv = std::get<Vector2f>(aContext.ReadInputPin(myUVIn_ID).data);
	std::string uvToString;
	if (uv.x == -1 && uv.y == -1)
	{
		uvToString = "scaledUV";
	}
	else
	{
		uvToString = aContext.ParseFromPin(myUVIn_ID);
	}


	std::string imageVar = aContext.GetShaderParseCompiler()->GetOrRegisterImage((ShaderSource)this, myTextureResource, uvToString);

	if (aID == myColOut_ID)
	{
		return ParsedData("float4", imageVar);
	}
	else if (aID == myXOut_ID)
	{
		imageVar += ".x";
	}
	else if (aID == myYOut_ID)
	{
		imageVar += ".y";
	}
	else if (aID == myZOut_ID)
	{
		imageVar += ".z";

	}
	else if (aID == myWOut_ID)
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



