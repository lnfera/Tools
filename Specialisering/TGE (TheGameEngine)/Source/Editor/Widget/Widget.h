#pragma once
#include "../EditorCommon.h"
namespace Tga
{
	class Widget
	{
	public:
		Widget(int aUniqueID) : myID(aUniqueID) {};
		virtual ~Widget() {};

		virtual bool Update(EditorContext& /*aContext*/) = 0;

		const int GetID() const { return myID; }
		bool GetIsOpen() { return myIsOpen; }
	protected:
		const int myID;
		bool myIsOpen = true;
	};
}

//#include "../Widget.h"
//namespace Tga
//{
//	class Template : public Widget
//	{
//	public:
//		Template(int aUniqueID) : Widget(aUniqueID) {};
//		~Template() {};
//
//		bool Update(EditorContext& /*aContext*/) override;
//	};
//}

//void Tga::Template::Update(EditorContext& aContext)
// {
//	std::string lable = "Template##" + std::to_string(myID);
//	if (ImGui::Begin(lable.c_str(), &myIsOpen))
//	{
//		ImGui::Text("some text");
//	}
//	ImGui::End();
//	
//	return myIsOpen;
//}
