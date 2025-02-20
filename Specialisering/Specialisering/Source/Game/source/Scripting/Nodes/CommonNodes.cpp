#include "CommonNodes.h"
#include <Scripting/ScriptNodeTypeRegistry.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptExecutionContext.h>
using namespace Tga;

void Tga::RegisterCommonNodes()
{
	ScriptNodeTypeRegistry::RegisterType<StartNode>("Common/Start", "Runs on starts.");
}

void Tga::StartNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin outputPin = {};
	outputPin.dataType = ScriptLinkDataType::Flow;
	outputPin.name = ScriptStringRegistry::RegisterOrGetString("Start");
	outputPin.node = aContext.GetNodeId();
	outputPin.role = ScriptPinRole::Output;

	myOutputPin = aContext.FindOrCreatePin(outputPin);
}
ScriptNodeResult Tga::StartNode::Execute(ScriptExecutionContext& aContext, ScriptPinId) const
{
	aContext.TriggerOutputPin(myOutputPin);

	return ScriptNodeResult::Finished;
}
