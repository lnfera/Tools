#pragma once

#include <vector>
#include <Scripting/ScriptCommon.h>
namespace Tga
{
	struct ScriptEditorSelection
	{
		std::vector<ScriptNodeId> mySelectedNodes;
		std::vector<ScriptLinkId> mySelectedLinks;
	};
} // namespace Tga