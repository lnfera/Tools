#pragma once
#include <string>
#include "ScriptCommon.h"

#include <Shader/ShaderCompiler.h>
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

		ShaderParseCompiler* myShaderParseCompiler = nullptr;

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


		/// <summary>
		/// Reads the information from a node by turning it and it's chain into a string
		/// </summary>
		/// <param name="aInputPin"></param>
		/// <returns></returns>
		std::string ParseFromPin(/*ShaderParseCompiler& aCompiler,*/ScriptPinId aInputPin);

		const ScriptPin& GetPin(ScriptPinId aID);


		ShaderParseCompiler* GetShaderParseCompiler() { return myShaderParseCompiler; }
		void SetShaderParseCompiler(ShaderParseCompiler* aCompiler) { myShaderParseCompiler = aCompiler; }
	};
}