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

	class PixelInputNodes : public ScriptNodeBase
	{
		Tga::ScriptPinId myPositionOut_Id;
		Tga::ScriptPinId myWorldPositionOut_Id;
		Tga::ScriptPinId myDepthOut_Id;
		Tga::ScriptPinId myNormalOut_Id;
	public:
		void Init(const Tga::ScriptCreationContext& aContext);
		ParsedData ParseInputPin(Tga::ScriptExecutionContext&, ScriptPinId) const;
	};
}