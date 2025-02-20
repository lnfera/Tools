#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
struct ID3D11ShaderResourceView;

namespace Tga
{
	class TextureResource;

	void RegisterShaderOutNodes();

	class ShaderOutputNode : public ScriptNodeBase
	{
		ScriptPinId myXIn_ID;
		ScriptPinId myYIn_ID;
		ScriptPinId myZIn_ID;
		ScriptPinId myWIn_ID;
	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }
		void CustomUi(float aSize) override;
	};

	class ImageNode : public ScriptNodeBase
	{
		mutable ID3D11ShaderResourceView* myTextureResource = nullptr;
		mutable std::string myTexturePath = "";
		ScriptPinId myXIn_ID;
	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptNodeResult Execute(ScriptExecutionContext& aContext, ScriptPinId) const override;
		bool ShouldExecuteAtStart() const override { return true; }
		void CustomUi(float aSize) override;

		void LoadFromJson(const ScriptJson&) override;
		void WriteToJson(ScriptJson&) const override;
	};

}