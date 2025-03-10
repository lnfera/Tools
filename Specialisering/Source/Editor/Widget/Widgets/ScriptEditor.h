#pragma once
#include "../Widget.h"
#include "NodeScripting/ScriptEditorSelection.h"
#include "NodeScripting/NodeEditorCommon.h"

#include <map>
#include <memory>
#include <string_view>

struct ImFont;
class TextEditor;

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
		~ScriptEditor();

		bool Update(EditorContext& aContext) override;
	private:
		void RenderTopRow(EditorContext& aContext);
		void RenderEditor(EditorContext& aContext);
		void RenderPopups(EditorContext& aContext);
		void ImGuiShortCuts(EditorContext& aContext);
		Tga::ScriptNodeBase* GetNode(int aID);

		void ClearCopiedSelection();
	private:

		float mySizeMod = 1.f;

		char myNewScriptName[32];
		bool myNewShaderIsScript = false;
		bool myCreateNewCalled = false;

		std::string_view myActiveScript;
		std::shared_ptr<MoveNodesCommand> myInProgressMove;
		TextEditor* myTextEditor = nullptr;

		ImFont* myNodeFont = nullptr;

		std::vector<ScriptNodeBase*> myCopiedSelection;
	};
}
