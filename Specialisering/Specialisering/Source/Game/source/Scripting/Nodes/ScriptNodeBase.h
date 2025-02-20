#pragma once
#include <memory>
#include "../ScriptCommon.h"
#include <tge/math/color.h>
#include <imgui/imgui.h>

typedef std::pair<std::string, std::string> ParsedData;

namespace Tga
{
	class ShaderParseCompiler;

	class ScriptNodeRuntimeInstanceBase
	{
	public:
		virtual ~ScriptNodeRuntimeInstanceBase() {}
	};
	class ScriptNodeBase
	{
	public:
		virtual ~ScriptNodeBase() {}
		virtual void Init(const Tga::ScriptCreationContext& aContext) = 0;

		virtual std::unique_ptr<ScriptNodeRuntimeInstanceBase> CreateRuntimeInstanceData() const { return nullptr; }
		// todo custom editor UI api? also allow it to edit number of pins and be notified on connections
		// would be nice to be able to have a variable number of pins for example!

		virtual void CustomUi(float /*aSizeMod*/) 
		{
			__noop;
		}

		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const { return {}; }

		/// <summary>
		/// Returns a parsed version of the node for the shadercompie.
		/// </summary>
		/// <returns></returns>
		virtual ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const { return ParsedData("float", "0000.f"); }

		virtual void LoadFromJson(const ScriptJson&) { __noop; }
		virtual void WriteToJson(ScriptJson&) const { __noop; }

		virtual ScriptNodeResult Execute(ScriptExecutionContext&, ScriptPinId) const { return ScriptNodeResult::Finished; }

		// @todo change the bool to a priority score
		virtual bool ShouldExecuteAtStart() const { return false; }

		Vector4f myTitleBarCol			= { 100.0f , 100.0f, 100.0f, 255.f};
		Vector4f myTitleBarColHovered	= { 230.0f , 230.f, 230.f, 255.f};
		Vector4f myTitleBarColSelected	= { 255.0f , 255.f, 255.f, 255.f};

	};
}