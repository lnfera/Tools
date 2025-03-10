#pragma once
#include "ScriptNodeBase.h"
namespace Tga
{
	class TextureResource;


	void RegisterInputNodes();


	//class  FresnelNode : public ScriptNodeBase
	//{
	//public:
	//	FresnelNode();
	//	~FresnelNode();

	//	void Init(const Tga::ScriptCreationContext& aContext);

	//	ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

	//};

	class PixelInputNode : public ScriptNodeBase
	{
		Tga::ScriptPinId myPositionOut_Id;
		Tga::ScriptPinId myWorldPositionOut_Id;
		Tga::ScriptPinId myDepthOut_Id;
		Tga::ScriptPinId myNormalOut_Id;
	public:
		PixelInputNode* Clone() override
		{
			PixelInputNode* clone = new PixelInputNode();
			return clone;
		};
		void Init(const Tga::ScriptCreationContext& aContext);
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};
	class TextureCoordinateNode : public ScriptNodeBase
	{
		Tga::ScriptPinId myScaledUVOutput;
		Tga::ScriptPinId myScreenPosOutput;
	public:
		TextureCoordinateNode* Clone() override
		{
			TextureCoordinateNode* clone = new TextureCoordinateNode();
			return clone;
		};

		void Init(const Tga::ScriptCreationContext& aContext);
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};


	class ImageNode : public ScriptNodeBase
	{
	private:
		mutable Tga::TextureResource* myTextureResource = nullptr;
		mutable std::string myTexturePath = "";

		ScriptPinId myUVIn_ID;

		ScriptPinId myXOut_ID;
		ScriptPinId myYOut_ID;
		ScriptPinId myZOut_ID;
		ScriptPinId myWOut_ID;

		ScriptPinId myColOut_ID;

	public:
		ImageNode* Clone() override
		{
			ImageNode* clone = new ImageNode();
			clone->myTexturePath = myTexturePath;
			clone->myTextureResource = myTextureResource;
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		void CustomUiOverlaped(float aSize) override;

		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

		void LoadFromJson(const ScriptJson&) override;
		void WriteToJson(ScriptJson&) const override;
	};
}