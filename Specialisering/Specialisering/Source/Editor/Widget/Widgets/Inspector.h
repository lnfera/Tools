#pragma once
#include "../Widget.h"
#include <tge/math/Vector.h>
#include <Tools/Commands/TransformCommand.h>
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

		TransformCommand myTransformCommand;
		Vector3f myRotationalEuler;
		bool myIsEditingMatrix = false;
	};
}