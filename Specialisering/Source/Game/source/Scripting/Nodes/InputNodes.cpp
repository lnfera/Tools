#include "InputNodes.h"
#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptCreationContext.h>

using namespace Tga;
void Tga::RegisterInputNodes()
{
	//Tga::ScriptNodeTypeRegistry::RegisterType<FresnelNode>("Input/Fresnel", "Creates values using the viewing direction of the object");
	Tga::ScriptNodeTypeRegistry::RegisterType<PixelInputNode>("Input/PixelInput", "Reads the input data from the pixel");
	Tga::ScriptNodeTypeRegistry::RegisterType<TextureCoordinateNode>("Input/TextureCoords", "Reads uv values");
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
