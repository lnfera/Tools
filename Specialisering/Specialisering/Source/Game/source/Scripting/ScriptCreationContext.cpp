#include "ScriptCreationContext.h"

#include "Script.h"

#include <cassert>

using namespace Tga;

ScriptCreationContext::ScriptCreationContext(Script& aScript, ScriptNodeId aNodeId)
	: myScript(aScript)
	, myNodeId(aNodeId)
{}

ScriptPinId ScriptCreationContext::FindOrCreatePin(ScriptPin aPinData) const
{
	assert(aPinData.name.id != ScriptStringId::InvalidId);
	assert(aPinData.node == myNodeId);

	// Wrong Default Value on Pin
	assert(aPinData.role != ScriptPinRole::Input || aPinData.dataType != ScriptLinkDataType::Bool	|| std::holds_alternative<bool>(aPinData.defaultValue.data));
	assert(aPinData.role != ScriptPinRole::Input || aPinData.dataType != ScriptLinkDataType::Int	|| std::holds_alternative<int>(aPinData.defaultValue.data));
	assert(aPinData.role != ScriptPinRole::Input || aPinData.dataType != ScriptLinkDataType::Float	|| std::holds_alternative<float>(aPinData.defaultValue.data));

	size_t count;
	const ScriptPinId* pins;
	if (aPinData.role == ScriptPinRole::Input)
	{
		pins = myScript.GetInputPins(myNodeId, count);
	}
	else
	{
		pins = myScript.GetOutputPins(myNodeId, count);
	}

	for (int i = 0; i < count; i++)
	{
		ScriptPinId id = pins[i];
		const ScriptPin& oldPin = myScript.GetPin(id);
		if (oldPin.name == aPinData.name)
		{
			if (oldPin.overridenValue.data.index() == aPinData.defaultValue.data.index())
			{
				aPinData.overridenValue = oldPin.overridenValue;
			}

			myScript.SetPin(id, aPinData);
			return id;
		}
	}

	return myScript.CreatePin(aPinData);
}
