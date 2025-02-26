#pragma once
#include "../Widget.h"
namespace Tga
{
	class ConsoleWidget : public Widget
	{
	public:
		ConsoleWidget(int aUniqueID) : Widget(aUniqueID) {};
		~ConsoleWidget() {};

		bool Update(EditorContext& /*aContext*/) override;
	};
}
