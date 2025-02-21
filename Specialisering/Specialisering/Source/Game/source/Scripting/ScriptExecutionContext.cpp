#include "ScriptExecutionContext.h"

#include "Script.h"
#include "ScriptRuntimeInstance.h"
using namespace Tga;

const ScriptUpdateContext& ScriptExecutionContext::GetUpdateContext()
{
	return myUpdateContext;
}

ScriptExecutionContext::ScriptExecutionContext(ScriptRuntimeInstance& aScriptRuntimeInstance, const ScriptUpdateContext& aUpdateContext, ScriptNodeId nodeId, ScriptNodeRuntimeInstanceBase* aNodeRuntimeInstance)
	: myScriptRuntimeInstance(aScriptRuntimeInstance)
	, myUpdateContext(aUpdateContext)
	, myNodeId(nodeId)
	, myNodeRuntimeInstance(aNodeRuntimeInstance)
{}

ScriptExecutionContext::~ScriptExecutionContext()
{
	const Script& script = myScriptRuntimeInstance.GetScript();

	for (int i = 0; i < myTriggeredOutputCount; i++)
	{
		ScriptPinId pinId = myTriggeredOutputQueue[i];

		size_t count;
		const ScriptLinkId* linkIds = script.GetConnectedLinks(pinId, count);

		assert("Trying to trigger an output pin that isn't of type flow" && script.GetPin(pinId).dataType == ScriptLinkDataType::Flow);
		assert("Only one link allowed on Flow out pins" && count <= 1);

		if (count == 0)
			return;

		const ScriptLink& link = script.GetLink(linkIds[0]);
		ScriptPinId targetPinId = link.targetPinId;
		const ScriptPin& targetPin = script.GetPin(targetPinId);

		ScriptNodeId nodeId = targetPin.node;

		ScriptExecutionContext executionContext(myScriptRuntimeInstance, myUpdateContext, nodeId, myScriptRuntimeInstance.GetRuntimeInstance(nodeId));

		const ScriptNodeBase& node = script.GetNode(nodeId);

		ScriptNodeResult result = node.Execute(executionContext, targetPinId);
		if (result == ScriptNodeResult::KeepRunning)
		{
			myScriptRuntimeInstance.ActivateNode(nodeId);
		}
		else
		{
			myScriptRuntimeInstance.DeactivateNode(nodeId);
		}
	}
}

void ScriptExecutionContext::TriggerOutputPin(ScriptPinId aPinId)
{
	assert(myTriggeredOutputCount < MAX_TRIGGERED_OUTPUTS);

	if (myTriggeredOutputCount < MAX_TRIGGERED_OUTPUTS)
	{
		myTriggeredOutputQueue[myTriggeredOutputCount] = aPinId;
		myTriggeredOutputCount++;
	}
}

ScriptLinkData ScriptExecutionContext::ReadInputPin(ScriptPinId aPinId)
{
	const Script& script = myScriptRuntimeInstance.GetScript();

	size_t count;
	const ScriptLinkId* linkIds = script.GetConnectedLinks(aPinId, count);

	assert("Trying to read from a flow pin" && script.GetPin(aPinId).dataType != ScriptLinkDataType::Flow);
	assert("Trying to read from a pin with unknown type" && script.GetPin(aPinId).dataType != ScriptLinkDataType::UNKNOWN);
	assert("Only one link allowed on value input pins" && count <= 1);

	if (count == 0)
	{
		const ScriptPin& pin = script.GetPin(aPinId);
		if (!std::holds_alternative<std::monostate>(pin.overridenValue.data))
			return pin.overridenValue;

		return pin.defaultValue;
	}

	const ScriptLink& link = script.GetLink(linkIds[0]);
	ScriptPinId sourcePinId = link.sourcePinId;
	const ScriptPin& sourcePin = script.GetPin(sourcePinId);

	ScriptNodeId nodeId = sourcePin.node;

	ScriptExecutionContext executionContext(*this);
	executionContext.myNodeId = nodeId;
	executionContext.myNodeRuntimeInstance = myScriptRuntimeInstance.GetRuntimeInstance(nodeId);

	const ScriptNodeBase& node = script.GetNode(nodeId);

	return node.ReadPin(executionContext, sourcePinId);
}

std::string Tga::ScriptExecutionContext::ParseFromPin(/*ShaderParseCompiler& aCompiler,*/ ScriptPinId aInputPin)
{
	const Script& script = myScriptRuntimeInstance.GetScript();

	size_t count;
	const ScriptLinkId* linkIds = script.GetConnectedLinks(aInputPin, count);

	assert("Trying to read from a flow pin" && script.GetPin(aInputPin).dataType != ScriptLinkDataType::Flow);
	assert("Trying to read from a pin with unknown type" && script.GetPin(aInputPin).dataType != ScriptLinkDataType::UNKNOWN);
	assert("Only one link allowed on value input pins" && count <= 1);

	// InputPin is not connected
	std::string output = "";
	if (count == 0)
	{
		const ScriptPin& pin = script.GetPin(aInputPin);

		std::variant<std::monostate, bool, int, float, ScriptStringId, Vector4f> data;


		if (!std::holds_alternative<std::monostate>(pin.overridenValue.data))
		{
			data = pin.overridenValue.data;
		}
		else
		{
			data = pin.defaultValue.data;
		}

		switch ((Tga::ScriptLinkData::VariantType)data.index())
		{
		case Tga::ScriptLinkData::VariantType::Int:
			output = std::to_string(std::get<int>(data));
			myShaderParseCompiler->RegisterVariable((ShaderSource)&pin, "int", output + ".f");

			break;
		case Tga::ScriptLinkData::VariantType::Float:
			output = std::to_string(std::get<float>(data));
			myShaderParseCompiler->RegisterVariable((ShaderSource)&pin, "float", output);
			break;
		case Tga::ScriptLinkData::VariantType::Float4:
		{
			// @todo there is a possible issue here with overriden values
			Vector4f val = std::get<Vector4f>(data);
			output = "float4(" + std::to_string(val.x) + ".f," + std::to_string(val.y) + ".f," + std::to_string(val.z) + ".f," + std::to_string(val.w) + ".f)";

			myShaderParseCompiler->RegisterVariable((ShaderSource)&pin, "float4", output);
		}
		break;

		default:
			output = "0";
			myShaderParseCompiler->RegisterVariable((ShaderSource)&pin, "float", output);
			break;
		}
		return output;
	}

	const ScriptLink& link = script.GetLink(linkIds[0]);
	ScriptPinId sourcePinId = link.sourcePinId;
	const ScriptPin& sourcePin = script.GetPin(sourcePinId);

	ScriptNodeId nodeId = sourcePin.node;

	ScriptExecutionContext executionContext(*this);
	executionContext.myNodeId = nodeId;
	executionContext.myNodeRuntimeInstance = myScriptRuntimeInstance.GetRuntimeInstance(nodeId);

	const ScriptNodeBase& node = script.GetNode(nodeId);


	ShaderSource shaderSource = (ShaderSource)&sourcePin;
	if (myShaderParseCompiler->isVariableRegistered(shaderSource))
	{
		// substitute with registered variable name
	}
	else
	{
		ParsedData parseNodeData = node.ParseInputPin(executionContext, sourcePinId);
		myShaderParseCompiler->RegisterVariable((ShaderSource)&sourcePin, parseNodeData.first, parseNodeData.second);

	}
	return myShaderParseCompiler->GetRegisteredVariableName(shaderSource);

}

const ScriptPin& Tga::ScriptExecutionContext::GetPin(ScriptPinId aID)
{
	// TODO: insert return statement here
	return myScriptRuntimeInstance.GetScript().GetPin(aID);
}


ScriptNodeRuntimeInstanceBase* ScriptExecutionContext::GetRuntimeInstanceData()
{
	return myNodeRuntimeInstance;
}
