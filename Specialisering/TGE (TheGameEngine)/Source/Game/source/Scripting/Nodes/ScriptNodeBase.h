#pragma once
#include <memory>
#include "../ScriptCommon.h"
#include <tge/math/color.h>
#include <imgui/imgui.h>
namespace Tga
{
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
			//ImGui::ImageButton(nullptr, ImVec2(50 * aSizeMod, 50 * aSizeMod));
		}

		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const { return {}; }
		virtual void LoadFromJson(const ScriptJson&) { __noop; }
		virtual void WriteToJson(ScriptJson&) const { __noop; }

		virtual ScriptNodeResult Execute(ScriptExecutionContext&, ScriptPinId) const { return ScriptNodeResult::Finished; }

		//todo change the bool to a priority score
		virtual bool ShouldExecuteAtStart() const { return false; }
	};
}