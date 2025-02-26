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
	// converters
	Tga::ScriptNodeTypeRegistry::RegisterType<Converter>("Math/Converters/Converter", "Simple math functions float");
	Tga::ScriptNodeTypeRegistry::RegisterType<Converter4>("Math/Converters/Converter4", "Simple math functions for float4");
	
	//Splitter
	Tga::ScriptNodeTypeRegistry::RegisterType<Vector4ToFloatNode>("Math/Splitter/Vector4ToFloat", "Splits Vector4f into four floats");
	// variables
	Tga::ScriptNodeTypeRegistry::RegisterType<Vector4fNode>("Math/Variables/Vector4f", "A Vector4 on a node");
	// functions
	Tga::ScriptNodeTypeRegistry::RegisterType<BlendFloatNode>("Math/Functions/Blend", "Lerps between two values");
	Tga::ScriptNodeTypeRegistry::RegisterType<BlendFloat4Node>("Math/Functions/Blend4", "Lerps between two values of a float4");
	Tga::ScriptNodeTypeRegistry::RegisterType<CosNode>("Math/Functions/Cosinus", "It does the cosinus on input val");
}
#pragma region Functions
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


void Tga::BlendFloatNode::Init(const ScriptCreationContext& aContext)
{
	myType = "float";

	//Input
	{
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Float;
			inputPin.node = aContext.GetNodeId();
			inputPin.role = ScriptPinRole::Input;
			inputPin.defaultValue = { 0.5f };
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Blend");

			myInputBlendFactor = aContext.FindOrCreatePin(inputPin);
		}
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Float;
			inputPin.node = aContext.GetNodeId();
			inputPin.role = ScriptPinRole::Input;

			inputPin.defaultValue = { 0.f };
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("A");
			myInput_A = aContext.FindOrCreatePin(inputPin);

			inputPin.defaultValue = { 1.f };
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("B");
			myInput_B = aContext.FindOrCreatePin(inputPin);
		}
	}
	// output
	{
		ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::Float;
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Output;
		inputPin.defaultValue = { 0.0f };
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");

		aContext.FindOrCreatePin(inputPin);
	}
}
Tga::ScriptLinkData Tga::BlendFloatNode::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	float blendFactor = std::get<float>(aContext.ReadInputPin(myInputBlendFactor).data);

	float valA = std::get<float>(aContext.ReadInputPin(myInput_A).data);
	float valB = std::get<float>(aContext.ReadInputPin(myInput_B).data);

	float out = (1.0f - valA) + blendFactor * valB;

	return { out };
}
ParsedData Tga::BlendFloatNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	std::string blendFactor = "clamp(" + aContext.ParseFromPin(myInputBlendFactor) + ",0,1)";

	std::string valA = aContext.ParseFromPin(myInput_A);
	std::string valB = aContext.ParseFromPin(myInput_B);

	std::string out = "(1.0f - " + blendFactor + ") * "+  valA + " + " + blendFactor + "* "+ valB;

	return { myType ,out };
}


void Tga::BlendFloat4Node::Init(const ScriptCreationContext& aContext)
{
	myType = "float4";

	//Input
	{
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Float;
			inputPin.node = aContext.GetNodeId();
			inputPin.role = ScriptPinRole::Input;
			inputPin.defaultValue = { 0.5f };
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Blend");

			myInputBlendFactor = aContext.FindOrCreatePin(inputPin);
		}
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Float4;
			inputPin.node = aContext.GetNodeId();
			inputPin.role = ScriptPinRole::Input;

			inputPin.defaultValue = { Vector4f()};
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("A");
			myInput_A = aContext.FindOrCreatePin(inputPin);

			inputPin.defaultValue = { Vector4f(1,1,1,1)};
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("B");
			myInput_B = aContext.FindOrCreatePin(inputPin);
		}
	}
	// output
	{
		ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::Float4;
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Output;
		inputPin.defaultValue = {  Vector4f() };
		inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");

		aContext.FindOrCreatePin(inputPin);
	}
}

Tga::ScriptLinkData Tga::BlendFloat4Node::ReadPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	float blendFactor = std::get<float>(aContext.ReadInputPin(myInputBlendFactor).data);

	Vector4f valA = std::get<Vector4f>(aContext.ReadInputPin(myInput_A).data);
	Vector4f valB = std::get<Vector4f>(aContext.ReadInputPin(myInput_B).data);

	Vector4f out = (Vector4f(1,1,1,1) - valA) + blendFactor * valB;

	return { out };
}

ParsedData Tga::BlendFloat4Node::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId) const
{
	std::string blendFactor = "clamp(" + aContext.ParseFromPin(myInputBlendFactor) + ",0,1)";

	std::string valA = aContext.ParseFromPin(myInput_A);
	std::string valB = aContext.ParseFromPin(myInput_B);

	std::string out = "(float4(1,1,1,1) - " + blendFactor + ")* " + valA + " + " + valB + " * " + blendFactor;;

	return { myType ,out };
}


#pragma endregion // Functions

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
	Vector4f vector = std::get<Vector4f>(aContext.ReadInputPin(myInputPin).data);
	if (aPinId == myOutput_X)
	{
		return { vector.x };
	}
	if (aPinId == myOutput_Y)
	{
		return { vector.y };
	}
	if (aPinId == myOutput_Z)
	{
		return { vector.z };
	}
	if (aPinId == myOutput_W)
	{
		return { vector.w };
	}

	return { 0 };
}

ParsedData Tga::Vector4ToFloatNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, ScriptPinId aPin) const
{
	std::string value = aContext.ParseFromPin(myInputPin);
	if (aPin == myOutput_X)
	{
		value += ".x";
	}
	if (aPin == myOutput_Y)
	{
		value += ".y";
	}
	if (aPin == myOutput_Z)
	{
		value += ".z";
	}
	if (aPin == myOutput_W)
	{
		value += ".w";
	}

	return ParsedData("float", value);
}

void Tga::Vector4fNode::Init(const Tga::ScriptCreationContext& aContext)
{
	// Input
	{
		Tga::ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::Float;
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Input;
		inputPin.defaultValue = { 0.f };

		{
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("X");
			myInput_X = aContext.FindOrCreatePin(inputPin);
		}
		{
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Y");
			myInput_Y = aContext.FindOrCreatePin(inputPin);
		}
		{
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Z");
			myInput_Z = aContext.FindOrCreatePin(inputPin);
		}
		{
			inputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("W");
			myInput_W = aContext.FindOrCreatePin(inputPin);
		}
	}
	// Output
	{
		Tga::ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float4;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { Vector4f() };

		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("Out");
		myOutputPin = aContext.FindOrCreatePin(outputPin);
	}
}

Tga::ScriptLinkData Tga::Vector4fNode::ReadPin(Tga::ScriptExecutionContext& aContext, Tga::ScriptPinId) const
{
	float x = std::get<float>(aContext.ReadInputPin(myInput_X).data);
	float y = std::get<float>(aContext.ReadInputPin(myInput_Y).data);
	float z = std::get<float>(aContext.ReadInputPin(myInput_Z).data);
	float w = std::get<float>(aContext.ReadInputPin(myInput_W).data);


	return { Vector4f(x,y,z,w) };
}

ParsedData Tga::Vector4fNode::ParseInputPin(Tga::ScriptExecutionContext& aContext, Tga::ScriptPinId) const
{
	std::string x = aContext.ParseFromPin(myInput_X);
	std::string y = aContext.ParseFromPin(myInput_Y);
	std::string z = aContext.ParseFromPin(myInput_Z);
	std::string w = aContext.ParseFromPin(myInput_W);

	std::string out = "float4(" + x + ", " + y + ", " + z + ", " + w + ")";

	return ParsedData("float4", out);
}
