#include "BufferNodes.h"
#include <tge/engine.h>
#include <Scripting/ScriptCreationContext.h>
#include <Scripting/ScriptExecutionContext.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/ScriptNodeTypeRegistry.h>
using namespace Tga;

void Tga::RegisterBufferNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<DeltaTimeNode>("Buffers/Time/DeltaTime", "The time between frames");
	Tga::ScriptNodeTypeRegistry::RegisterType<TotalTimeNode>("Buffers/Time/TotalTime", "The time since start until now");
}



void Tga::DeltaTimeNode::Init(const ScriptCreationContext& aContext)
{
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Float;
		outputPin.node = aContext.GetNodeId();
		outputPin.role = ScriptPinRole::Output;
		outputPin.defaultValue = { 0.f };
		outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("DeltaTime");

		aContext.FindOrCreatePin(outputPin);
	}
}

ScriptLinkData Tga::DeltaTimeNode::ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const
{
	return { TGE_I()->GetDeltaTime() };
}

void Tga::TotalTimeNode::Init(const ScriptCreationContext& aContext)
{
	ScriptPin outputPin = {};
	outputPin.dataType = ScriptLinkDataType::Float;
	outputPin.node = aContext.GetNodeId();
	outputPin.role = ScriptPinRole::Output;
	outputPin.defaultValue = { 0.f };
	outputPin.name = Tga::ScriptStringRegistry::RegisterOrGetString("TotalTime");

	aContext.FindOrCreatePin(outputPin);
}

ScriptLinkData Tga::TotalTimeNode::ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const
{
	return { TGE_I()->GetTotalTime() };
}
