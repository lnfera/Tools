#pragma once
#include <tge/math/Vector.h>
#include <EditorCommon.h>
namespace Tga
{
	class VisualGrid
	{
	public:
		VisualGrid(int aGridSize, float aCellSize);
		void Render(Vector3f aCameraPosition);

	private:
		int myGridSize = 10;
		float myCellSize = 100.f;
	};
}