#pragma once
#include "../Widget.h"
namespace Tga
{
	class GameObject;
	class Inspector : public Widget
	{
	public:
		Inspector(int aID) : Widget(aID) {};
		~Inspector() = default;
		virtual bool Update(EditorContext& /*aContext*/)override;
	protected:
		void DisplayComponentAddList(Tga::GameObject* aObject);
		/*std::vector<std::string> myComponentTypes;*/
	};
}