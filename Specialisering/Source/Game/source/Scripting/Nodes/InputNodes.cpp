#include "InputNodes.h"
#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptCreationContext.h>

using namespace Tga;
void Tga::RegisterInputNodes()
{
	//Tga::ScriptNodeTypeRegistry::RegisterType<FresnelNode>("Input/Fresnel", "Creates values using the viewing direction of the object");
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