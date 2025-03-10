#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
namespace Tga
{
	void RegisterCommonNodes();

	class StartNode : public ScriptNodeBase
	{
		ScriptPinId myOutputPin;
	public:
		StartNode* Clone() override
		{
			StartNode* clone = new StartNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }

	};

}