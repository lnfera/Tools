#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
#include <Shader/ShaderCompiler.h>
namespace Tga
{
	void RegisterBufferNodes();

	class DeltaTimeNode : public ScriptNodeBase
	{
	public:
		DeltaTimeNode* Clone() override
		{
			DeltaTimeNode* clone = new DeltaTimeNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override
		{
			return ParsedData("float", "Timings.y");
		}
	};
	class TotalTimeNode : public ScriptNodeBase
	{
	public:
		TotalTimeNode* Clone() override
		{
			TotalTimeNode* clone = new TotalTimeNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override
		{
			return ParsedData("float", "Timings.x");
		}
	};

}