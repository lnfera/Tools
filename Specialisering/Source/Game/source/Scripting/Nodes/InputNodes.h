#pragma once
#include "ScriptNodeBase.h"
namespace Tga
{
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
		void Init(const Tga::ScriptCreationContext& aContext);
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};
	class TextureCoordinateNode : public ScriptNodeBase
	{
		Tga::ScriptPinId myScaledUVOutput;
		Tga::ScriptPinId myScreenPosOutput;
	public:
		void Init(const Tga::ScriptCreationContext& aContext);
		ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};
}