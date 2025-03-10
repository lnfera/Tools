#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
#include <string>
namespace Tga
{
	void RegisterMathNodes();

	class CosNode : public ScriptNodeBase
	{
		ScriptPinId myInput_Id;
	public:
		CosNode* Clone() override
		{
			CosNode* clone = new CosNode();
			return clone;
		};

		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};


	class BlendFloatNode : public ScriptNodeBase
	{
	protected:
		std::string myType = "";
		ScriptPinId myInputBlendFactor;
		ScriptPinId myInput_A;
		ScriptPinId myInput_B;
	public:
		BlendFloatNode* Clone() override
		{
			BlendFloatNode* clone = new BlendFloatNode();
			return clone;
		};

		virtual void Init(const ScriptCreationContext& aContext) override;
		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		virtual ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};
	class BlendFloat4Node : public ScriptNodeBase
	{
	protected:

		std::string myType = "";
		ScriptPinId myInputBlendFactor;
		ScriptPinId myInput_A;
		ScriptPinId myInput_B;
	public:
		BlendFloat4Node* Clone() override
		{
			BlendFloat4Node* clone = new BlendFloat4Node();
			return clone;
		};
		virtual void Init(const ScriptCreationContext& aContext) override;
		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		virtual ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};

	class SaturateNode : public ScriptNodeBase
	{
		ScriptPinId myInput_Id;
	public:
		SaturateNode* Clone() override
		{
			SaturateNode* clone = new SaturateNode();
			return clone;
		};

		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};

	enum ConverterMode
	{
		Add = 0,
		Subtract,
		Multiply,
		Divide,
		Max,
		Min,
		Step,
		COUNT
	};
	class Converter : public ScriptNodeBase
	{
	protected:
		mutable ConverterMode myConverterMode = ConverterMode::Add;
		ScriptPinId myIn_A;
		ScriptPinId myIn_B;
		std::string myType = "";
	public:
		Converter* Clone() override
		{
			Converter* clone = new Converter();
			clone->myType = myType;
			return clone;
		};

		virtual void Init(const ScriptCreationContext& aContext) override;
		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		virtual ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

		virtual void LoadFromJson(const ScriptJson&);
		virtual void WriteToJson(ScriptJson&) const;
		virtual void CustomUiBelow(float aSizeMod);
	};
	class Converter2 : public Converter
	{
	public:
		Converter2* Clone() override
		{
			Converter2* clone = new Converter2();
			clone->myType = myType;
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
	class Converter4 : public Converter
	{
	public:
		Converter4* Clone() override
		{
			Converter4* clone = new Converter4();
			clone->myType = myType;
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
#pragma region Splitters
	class Vector4ToFloatNode : public ScriptNodeBase
	{
		ScriptPinId myInputPin;
		ScriptPinId myOutput_X;
		ScriptPinId myOutput_Y;
		ScriptPinId myOutput_Z;
		ScriptPinId myOutput_W;
	public:
		Vector4ToFloatNode* Clone() override
		{
			Vector4ToFloatNode* clone = new Vector4ToFloatNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
	class Vector2ToFloatNode : public ScriptNodeBase
	{
		ScriptPinId myInputPin;
		ScriptPinId myOutput_X;
		ScriptPinId myOutput_Y;
	public:
		Vector2ToFloatNode* Clone() override
		{
			Vector2ToFloatNode* clone = new Vector2ToFloatNode();
			return clone;
		};

		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
#pragma endregion
#pragma region Variables
	class Vector4fNode : public ScriptNodeBase
	{
		ScriptPinId myOutputPin;
		ScriptPinId myInput_X;
		ScriptPinId myInput_Y;
		ScriptPinId myInput_Z;
		ScriptPinId myInput_W;
	public:
		Vector4fNode* Clone() override
		{
			Vector4fNode* clone = new Vector4fNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
	class Vector2fNode : public ScriptNodeBase
	{
		ScriptPinId myOutputPin;
		ScriptPinId myInput_X;
		ScriptPinId myInput_Y;
	public:
		Vector2fNode* Clone() override
		{
			Vector2fNode* clone = new Vector2fNode();
			return clone;
		};

		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
	class FloatNode : public ScriptNodeBase
	{
		ScriptPinId myOutputPin;
		ScriptPinId myInputPin;
	public:
		FloatNode* Clone() override
		{
			FloatNode* clone = new FloatNode();
			return clone;
		};
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;

	};
#pragma endregion
}