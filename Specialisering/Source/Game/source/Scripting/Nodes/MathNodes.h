#pragma once
#include <Scripting/Nodes/ScriptNodeBase.h>
namespace Tga
{
	void RegisterMathNodes();

	class CosNode : public ScriptNodeBase
	{
		ScriptPinId myInput_Id;
	public:
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
		virtual void Init(const ScriptCreationContext& aContext) override;
		virtual ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		virtual ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

		virtual void LoadFromJson(const ScriptJson&);
		virtual void WriteToJson(ScriptJson&) const;
		virtual void CustomUiBelow(float aSizeMod);
	};

	class Converter4 : public Converter
	{
	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
		//std::string ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;

		//virtual void LoadFromJson(const ScriptJson&);
		//virtual void WriteToJson(ScriptJson&) const;
		//virtual void CustomUi(float aSizeMod);
	};

	class Vector4ToFloatNode : public ScriptNodeBase
	{
		ScriptPinId myInputPin;
		ScriptPinId myOutput_X;
		ScriptPinId myOutput_Y;
		ScriptPinId myOutput_Z;
		ScriptPinId myOutput_W;
	public:
		void Init(const ScriptCreationContext& aContext) override;
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const override;
	};
}