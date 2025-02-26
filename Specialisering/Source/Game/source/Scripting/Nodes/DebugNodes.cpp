#include "DebugNodes.h"
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptExecutionContext.h>

using namespace Tga;

void Tga::RegisterDebugNodes()
{
	ScriptNodeTypeRegistry::RegisterType<PrintTextNode>("Debug/Print", "Prints a text message into the console on execute.");
}


void Tga::PrintTextNode::Init(const ScriptCreationContext& aContext)
{
	{
		ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::Flow;
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("In");
		inputPin.node = aContext.GetNodeId();
		inputPin.role = ScriptPinRole::Input;

		aContext.FindOrCreatePin(inputPin);
	}
	{
		ScriptPin inputPin = {};
		inputPin.dataType = ScriptLinkDataType::String;
		inputPin.name = ScriptStringRegistry::RegisterOrGetString("Text");
		inputPin.node = aContext.GetNodeId();
		inputPin.defaultValue = { Tga::ScriptStringRegistry::RegisterOrGetString("text") };
		inputPin.role = ScriptPinRole::Input;

		myTextIn_Id = aContext.FindOrCreatePin(inputPin);
	}
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Flow;
		outputPin.name = ScriptStringRegistry::RegisterOrGetString("Out");
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;

		myOutputPin_Id = aContext.FindOrCreatePin(outputPin);
	}
}
ScriptNodeResult Tga::PrintTextNode::Execute(ScriptExecutionContext& aContext, ScriptPinId) const
{
	// Add print function

	ScriptLinkData data = aContext.ReadInputPin(myTextIn_Id);

	ScriptStringId stringId = std::get<ScriptStringId>(data.data);
	std::cout << ScriptStringRegistry::GetStringFromStringId(stringId) << "\n";
	aContext.TriggerOutputPin(myOutputPin_Id);

	return ScriptNodeResult::Finished;
}
