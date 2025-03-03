#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
struct ID3D11ShaderResourceView;

namespace Tga
{
	class TextureResource;

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

	class ImageNode : public ScriptNodeBase
	{
		mutable Tga::TextureResource* myTextureResource = nullptr;
		mutable std::string myTexturePath = "";

		ScriptPinId myUVIn_ID;

		ScriptPinId myXOut_ID;
		ScriptPinId myYOut_ID;
		ScriptPinId myZOut_ID;
		ScriptPinId myWOut_ID;

		ScriptPinId myColOut_ID;

	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		void CustomUiOverlaped(float aSize) override;

		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

		void LoadFromJson(const ScriptJson&) override;
		void WriteToJson(ScriptJson&) const override;
	};

}