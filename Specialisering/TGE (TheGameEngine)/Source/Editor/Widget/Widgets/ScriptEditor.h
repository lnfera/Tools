#pragma once
#include "../Widget.h"
#include "NodeScripting/ScriptEditorSelection.h"
#include "NodeScripting/NodeEditorCommon.h"

#include <map>
#include <memory>
#include <string_view>

namespace Tga
{
	class MoveNodesCommand;



	class ScriptEditor : public Widget
	{
	public:
		struct EditorScriptData
		{
			Script* script;
			ScriptEditorSelection selection = {};
			ImNodesEditorContext* nodeEditorContext = nullptr;
			ScriptPinId inProgressLinkPin = { ScriptPinId::InvalidId };
			ScriptNodeId hoveredNode = { ScriptNodeId::InvalidId };
			int latestSavedSequenceNumber = 0;
		};

		ScriptEditor(int aUniqueID);
		virtual ~ScriptEditor() {};

		bool Update(EditorContext& aContext) override;
	private:
		void RenderTopRow(EditorContext& aContext);
		void RenderEditor(EditorContext& aContext);
		void ImGuiShortCuts(EditorContext& aContext);
		Tga::ScriptNodeBase* GetNode(int aID);
	private:

		char myNewScriptName[32];
		bool myCreateNewCalled = false;

		std::string_view myActiveScript;
		std::shared_ptr<MoveNodesCommand> myInProgressMove;
	};
}
