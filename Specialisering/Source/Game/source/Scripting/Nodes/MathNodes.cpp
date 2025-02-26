#include "MathNodes.h"
#include <Scripting/ScriptCommon.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptJson.h>
#include <imgui/imgui.h>
void Tga::RegisterMathNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<Converter>("Math/Converter", "Simple math functions float");
	Tga::ScriptNodeTypeRegistry::RegisterType<Converter4>("Math/Converter4", "Simple math functions for float4");
	Tga::ScriptNodeTypeRegistry::RegisterType<CosNode>("Math/Cosinus", "It does the cosinus on input val");
}

void Tga::CosNode::Init(const ScriptCreationContext& aContext)
{
	{
		ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::Float;
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Input;
		inputPin.defaultValue = { 0.f };
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString(" ");

		myInput_Id = aContext.FindOrCreatePin(inputPin);
	}
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.f };
		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");

		aContext.FindOrCreatePin(outputPin);
	}
}

Tga::ScriptLinkData Tga::CosNode::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	float val = std::get<float>(aContext.ReadInputPin(myInput_Id).data);
	return { cosf(val) };
}

ParsedData Tga::CosNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{

	ParsedData out("float", "cos(" + aContext.ParseFromPin(myInput_Id) + ")");
	return out;
}

#pragma region Converter
void Tga::Converter::Init(const ScriptCreationContext& aContext)
{
	myType = "float";

	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { 0.f };
	{
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("A");

		myIn_A = aContext.FindOrCreatePin(inputPin);
	}

	{
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("B");

		myIn_B = aContext.FindOrCreatePin(inputPin);
	}
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.f };
		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");

		aContext.FindOrCreatePin(outputPin);
	}
}

Tga::ScriptLinkData Tga::Converter::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	float valA = std::get<float>(aContext.ReadInputPin(myIn_A).data);
	float valB = std::get<float>(aContext.ReadInputPin(myIn_B).data);
	switch (myConverterMode)
	{
	case Tga::ConverterMode::Add:
		return { valA + valB };
		break;
	case Tga::ConverterMode::Subtract:
		return { valA - valB };
		break;
	case Tga::ConverterMode::Multiply:
		return { valA * valB };
		break;
	case Tga::ConverterMode::Divide:
		return { valA / valB };
	case Tga::ConverterMode::Max:
		return { std::max<float>(valA,valB) };
	case Tga::ConverterMode::Min:
		return { std::min<float>(valA,valB) };
		break;
	case Tga::ConverterMode::COUNT:
		break;
	default:
		return { 0 };
		break;
	}
	return { 0 };
}
ParsedData Tga::Converter::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	std::string valA = aContext.ParseFromPin(myIn_A);
	std::string valB = aContext.ParseFromPin(myIn_B);

	std::string value = "";

	switch (myConverterMode)
	{
	case Tga::ConverterMode::Add:
		value = valA + " +" + valB;
		break;
	case Tga::ConverterMode::Subtract:
		value = valA + " -" + valB;
		break;
	case Tga::ConverterMode::Multiply:
		value = valA + " *" + valB;
		break;
	case Tga::ConverterMode::Divide:
		value = valA + " /" + valB;
		break;
	case Tga::ConverterMode::Min:
		value = "min(" + valA + " ," + valB + ")";
		break;
	case Tga::ConverterMode::Max:
		value = "max(" + valA + " ," + valB + ")";
		break;
	case Tga::ConverterMode::COUNT:
	default:
		value = "0";
		break;
	}

	ParsedData out(myType, value);
	return out;
}

void Tga::Converter::LoadFromJson(const ScriptJson& aJson)
{
	myConverterMode = (ConverterMode)aJson.json["mode"];
}
void Tga::Converter::WriteToJson(ScriptJson& aJson) const
{
	aJson.json["mode"] = (int)myConverterMode;
}
void Tga::Converter::CustomUiBelow(float aSizeMod)
{
	ImGui::SetNextItemWidth(aSizeMod * 85.f);

	ImGui::NewLine();
	int mode = (int)myConverterMode;

	const char* converterModeStr[ConverterMode::COUNT][32] = {
			" Add",
			" Subtract",
			" Multiply",
			" Divide",
			" Max",
			" Min"
	};

	//@todo add tooltip when selecting
	//const char* converterTooltip[ConverterMode::COUNT][32] = {
	//		" Adds the two values together",
	//		" Subtract the first value with the second",
	//		" Multiply the two values",
	//		" Divide the first value with the second",
	//		" Returns the max value of the two values",
	//		" Returns the min value of the two values"
	//};

	if (ImGui::Combo("##mode", &mode, converterModeStr[0], IM_ARRAYSIZE(converterModeStr)))
	{
		myConverterMode = (ConverterMode)mode;
	}
}


void Tga::Converter4::Init(const ScriptCreationContext& aContext)
{
	myType = "float4";
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float4;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { 0.f };
	{
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("A");

		myIn_A = aContext.FindOrCreatePin(inputPin);
	}

	{
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("B");

		myIn_B = aContext.FindOrCreatePin(inputPin);
	}
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float4;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.f };
		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");

		aContext.FindOrCreatePin(outputPin);
	}
}
Tga::ScriptLinkData Tga::Converter4::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	Vector4f valA = std::get<Vector4f>(aContext.ReadInputPin(myIn_A).data);
	Vector4f valB = std::get<Vector4f>(aContext.ReadInputPin(myIn_B).data);
	switch (myConverterMode)
	{
	case Tga::ConverterMode::Add:
		return { valA + valB };
		break;
	case Tga::ConverterMode::Subtract:
		return { valA - valB };
		break;
	case Tga::ConverterMode::Multiply:
		return { valA * valB };
		break;
	case Tga::ConverterMode::Divide:
		return { valA / valB };
		break;
	case Tga::ConverterMode::Max:
		return { Vector4f(std::max<float>(valA.x,valB.x),std::max<float>(valA.y,valB.y), std::max<float>(valA.z,valB.z),std::max<float>(valA.w,valB.w)) };
		break;
	case Tga::ConverterMode::Min:
		return { Vector4f(std::min<float>(valA.x,valB.x),std::min<float>(valA.y,valB.y), std::min<float>(valA.z,valB.z),std::min<float>(valA.w,valB.w)) };
		break;
	case Tga::ConverterMode::COUNT:
		break;
	default:
		return { 0 };
		break;
	}
	return { Vector4f() };
}
//std::string Tga::Converter4::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
//{
//	//auto* shaderContext = aContext.GetShaderParseCompiler();
//
//	std::string valA = VAR(aContext.ParseFromPin(myIn_A));
//	std::string valB = VAR(aContext.ParseFromPin(myIn_B));
//	switch (myConverterMode)
//	{
//	case Tga::ConverterMode::Add:
//		return { valA + "+" + valB };
//	case Tga::ConverterMode::Subtract:
//		return { valA + "-" + valB };
//	case Tga::ConverterMode::Multiply:
//		return { valA + "*" + valB };
//	case Tga::ConverterMode::Divide:
//		return { valA + "/" + valB };
//	case Tga::ConverterMode::Max:
//		return {"max(" + valA + "," + valB + ")"};	
//	case Tga::ConverterMode::Min:
//		return {"min(" + valA + "," + valB + ")"};
//	case Tga::ConverterMode::COUNT:
//		break;
//	default:
//		std::cout << 
//		break;
//	}
//	return "0";
//}



#pragma endregion // Converter

void Tga::Vector4ToFloatNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin inputPin = {};
	inputPin.dataType = ScriptLinkDataType::Float4;
	inputPin.node = aContext.GetNodeId();
	inputPin.role = ScriptPinRole::Input;
	inputPin.defaultValue = { Vector4f() };
	{
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("In");

		myInputPin = aContext.FindOrCreatePin(inputPin);
	}

	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.f };

		{
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("X");
			myOutput_X = aContext.FindOrCreatePin(outputPin);
		}
		{
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Y");
			myOutput_Y = aContext.FindOrCreatePin(outputPin);
		}
		{
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Z");
			myOutput_Z = aContext.FindOrCreatePin(outputPin);
		}
		{
			outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("W");
			myOutput_W = aContext.FindOrCreatePin(outputPin);
		}
	}
}

Tga::ScriptLinkData Tga::Vector4ToFloatNode::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId aPinId) const
{
	aContext; aPinId;
	//if()

	return ScriptLinkData();
}
