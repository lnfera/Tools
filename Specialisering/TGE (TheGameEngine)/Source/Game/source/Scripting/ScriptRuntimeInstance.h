#pragma once

#include "Script.h"
#include "Nodes/ScriptNodeBase.h"
#include <vector>

namespace Tga
{
	class ScriptUpdateContext;

	class ScriptRuntimeInstance
	{
		std::shared_ptr<const Script> myScript;

		std::vector<ScriptNodeId> myActiveNodes;
		std::vector<std::unique_ptr<ScriptNodeRuntimeInstanceBase>> myNodeInstances;
	public:

		ScriptRuntimeInstance(std::shared_ptr<const Script>& aScript);
		void Init();
		void Update(const ScriptUpdateContext& aContext);
		void TriggerNode(const ScriptUpdateContext& aContext, const ScriptNodeId aNodeId);
		void TriggerPin(ScriptPinId aPin, const ScriptUpdateContext& aContext);

		const Script& GetScript() const;
		ScriptNodeRuntimeInstanceBase* GetRuntimeInstance(ScriptNodeId aNodeId);

		void ActivateNode(ScriptNodeId aNodeId);
		void DeactivateNode(ScriptNodeId aNodeId);

	};
}; // namespace Tga