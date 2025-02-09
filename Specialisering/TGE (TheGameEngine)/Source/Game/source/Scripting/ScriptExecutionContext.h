#pragma once
#include "ScriptCommon.h"
namespace Tga
{
	class ScriptExecutionContext
	{
		static constexpr int MAX_TRIGGERED_OUTPUTS = 8;

		ScriptRuntimeInstance& myScriptRuntimeInstance;
		const ScriptUpdateContext& myUpdateContext;
		ScriptNodeId myNodeId;
		ScriptNodeRuntimeInstanceBase* myNodeRuntimeInstance;

		ScriptPinId myTriggeredOutputQueue[MAX_TRIGGERED_OUTPUTS];
		int myTriggeredOutputCount;
	public:
		ScriptExecutionContext(ScriptRuntimeInstance& aScriptRuntimeInstance, const ScriptUpdateContext& aUpdateContext, ScriptNodeId aNodeId, ScriptNodeRuntimeInstanceBase* aNodeRuntimeInstance);
		~ScriptExecutionContext();
		const ScriptUpdateContext& GetUpdateContext();
		ScriptNodeRuntimeInstanceBase* GetRuntimeInstanceData();

		/// <summary>
		/// Triggers and output pin. The execution is deferred until the ScriptExecutionContext is destroyed.
		/// </summary>
		/// <param name="outputPin"></param>
		void TriggerOutputPin(ScriptPinId aOutputPin);

		/// <summary>
		/// Reads an input pin. This reading functions to be called on the corresponding node immediately. 
		/// </summary>
		/// <param name="inputPin"></param>
		/// <returns></returns>
		ScriptLinkData ReadInputPin(ScriptPinId aInputPin);

	};
}