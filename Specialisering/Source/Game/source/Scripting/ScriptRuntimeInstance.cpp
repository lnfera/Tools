#include "ScriptRuntimeInstance.h"
#include "ScriptExecutionContext.h"
using namespace Tga;

ScriptRuntimeInstance::ScriptRuntimeInstance(std::shared_ptr<const Script>& aScript)
	: myScript(aScript)
{
	assert(aScript);
}

void ScriptRuntimeInstance::Init()
{
	ScriptNodeId lastNodeId = myScript->GetLastNodeId();
	myNodeInstances.resize(lastNodeId.id + 1);

	for (ScriptNodeId currentNodeId = myScript->GetFirstNodeId();
		currentNodeId.id != ScriptNodeId::InvalidId;
		currentNodeId = myScript->GetNextNodeId(currentNodeId))
	{
		const ScriptNodeBase& node = myScript->GetNode(currentNodeId);
		myNodeInstances[currentNodeId.id] = node.CreateRuntimeInstanceData();

		if (node.ShouldExecuteAtStart())
		{
			myActiveNodes.push_back(currentNodeId);
		}

		//const_cast<ScriptNodeBase*>(&node)->myScriptInstance = this;
		//const_cast<ScriptNodeBase*>(&node)->myID = currentNodeId;
	}
}

void ScriptRuntimeInstance::Update(const ScriptUpdateContext& aUpdateContext)
{
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		ScriptNodeId nodeId = myActiveNodes[i];
		ScriptExecutionContext executionContext(*this, aUpdateContext, nodeId, myNodeInstances[nodeId.id].get());
		const ScriptNodeBase& node = myScript->GetNode(nodeId);

		ScriptNodeResult result = node.Execute(executionContext, { ScriptPinId::InvalidId });
		if (result == ScriptNodeResult::Finished)
		{
			myActiveNodes.erase(begin(myActiveNodes) + i);
			i--;
		}
	}
}

void Tga::ScriptRuntimeInstance::TriggerNode(const ScriptUpdateContext& aContext, const ScriptNodeId aNodeId)
{

	const ScriptNodeBase& node = myScript->GetNode(aNodeId);
	ScriptExecutionContext executionContext(*this, aContext, aNodeId, myNodeInstances[aNodeId.id].get());
	ScriptNodeResult result = node.Execute(executionContext, { ScriptPinId::InvalidId });
	if (result == ScriptNodeResult::KeepRunning)
	{
		ActivateNode(aNodeId);
	}

}

void ScriptRuntimeInstance::TriggerPin(ScriptPinId aPinId, const ScriptUpdateContext& aUpdateContext)
{
	ScriptPin pin = myScript->GetPin(aPinId);
	ScriptNodeId nodeId = pin.node;
	ScriptExecutionContext executionContext(*this, aUpdateContext, nodeId, myNodeInstances[nodeId.id].get());
	const ScriptNodeBase& node = myScript->GetNode(nodeId);

	assert(pin.dataType == ScriptLinkDataType::Flow);
	if (pin.role == ScriptPinRole::Input)
	{
		ScriptNodeResult result = node.Execute(executionContext, aPinId);
		if (result == ScriptNodeResult::KeepRunning)
		{
			ActivateNode(nodeId);
		}
		else
		{
			DeactivateNode(nodeId);
		}
	}
	else
	{
		executionContext.TriggerOutputPin(aPinId);
	}
}

const Script& ScriptRuntimeInstance::GetScript() const
{
	return *myScript;
}

ScriptNodeRuntimeInstanceBase* ScriptRuntimeInstance::GetRuntimeInstance(ScriptNodeId NodeId)
{
	assert("Invalid node" && NodeId.id < myNodeInstances.size());
	return myNodeInstances[NodeId.id].get();
}

void ScriptRuntimeInstance::ActivateNode(ScriptNodeId nodeId)
{
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		if (myActiveNodes[i] == nodeId)
		{
			return;
		}
	}

	myActiveNodes.push_back(nodeId);
}

void ScriptRuntimeInstance::DeactivateNode(ScriptNodeId aNodeId)
{
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		if (myActiveNodes[i] == aNodeId)
		{
			myActiveNodes.erase(begin(myActiveNodes) + i);
			return;
		}
	}
}
