#pragma once

#include "ScriptCommon.h"

namespace Tga
{

	class Script;

	class ScriptCreationContext
	{
		Script& myScript;
		ScriptNodeId myNodeId;
	public:
		ScriptCreationContext(Script& aScript, ScriptNodeId aNodeId);
		ScriptNodeId GetNodeId() const { return myNodeId; };
		ScriptPinId FindOrCreatePin(ScriptPin aPinData) const;
	};

} // namespace Tga