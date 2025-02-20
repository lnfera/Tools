#pragma once
#include "ScriptNodeBase.h"
namespace Tga
{
	void RegisterDebugNodes();

	class PrintTextNode : public ScriptNodeBase
	{
		ScriptPinId myOutputPin_Id;
		ScriptPinId myTextIn_Id;
	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
	};
}