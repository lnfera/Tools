#pragma once
#include "../Widget.h"

namespace Tga
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy(int aUniqueID);
		virtual ~Hierarchy();

		bool Update(EditorContext& aContext) override;

	private:
	};
}