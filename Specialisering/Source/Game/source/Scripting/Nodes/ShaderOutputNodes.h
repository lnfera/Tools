#pragma once
#include <tge/shaders/ShaderCommon.h>
#include <Scripting/Nodes/ScriptNodeBase.h>
struct ID3D11ShaderResourceView;

namespace Tga
{
	void RegisterShaderOutNodes();


	class ShaderOutputNode : public ScriptNodeBase
	{
		ScriptPinId myColorIdIn_Id;
	public:
		ScriptNodeBase* Clone() override {return new ShaderOutputNode();};

		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }
		void CustomUiBelow(float aSize) override;
	};


	class PBROutputNode : public ScriptNodeBase
	{
		ScriptPinId myAlbedoIn_Id;
		ScriptPinId myNormalIn_Id;
		ScriptPinId myMaterialIn_Id;
		ScriptPinId myFxIn_Id;

		RenderMode myRenderMode;
	public:
		ScriptNodeBase* Clone() override 
		{ 
			PBROutputNode* clone = new PBROutputNode();
			clone->myRenderMode = myRenderMode;
			return clone; 
		};


		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }
		void CustomUiBelow(float aSizeMod) override;

		void LoadFromJson(const ScriptJson&) override;
		void WriteToJson(ScriptJson&) const override;
	};




}