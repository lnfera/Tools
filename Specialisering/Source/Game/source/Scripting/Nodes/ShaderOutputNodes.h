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
		/*ScriptPinId myXIn_ID;
		ScriptPinId myYIn_ID;
		ScriptPinId myZIn_ID;
		ScriptPinId myWIn_ID;*/
	public:
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
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }
		void CustomUiBelow(float aSizeMod) override;

		void LoadFromJson(const ScriptJson&) override;
		void WriteToJson(ScriptJson&) const override;
	};




}