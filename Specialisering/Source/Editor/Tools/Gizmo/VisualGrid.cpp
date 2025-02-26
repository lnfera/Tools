#include "VisualGrid.h"
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/LineDrawer.h>
#include <tge/primitives/LinePrimitive.h>
Tga::VisualGrid::VisualGrid(int aGridSize, float aCellsize) :
	myGridSize(aGridSize),
	myCellSize(aCellsize)
{
}

void Tga::VisualGrid::Render(Vector3f aCameraPosition)
{
	auto& lineDrawer = Tga::Engine::GetInstance()->GetGraphicsEngine().GetLineDrawer();

	Tga::LinePrimitive line;

	Vector3f offset = { myCellSize * myGridSize * -0.5f,0,myCellSize * myGridSize * -0.5f };
	offset.x += myCellSize * ((int)aCameraPosition.x / (int)(myCellSize));
	offset.z += myCellSize * ((int)aCameraPosition.z / (int)(myCellSize));


	// X Axis
	{
		line.color = { 1.f,0.f,0.f,1.0f };
		line.fromPosition = { 1000.f, 0.f, 0.f };
		line.toPosition = { -1000.f, 0.f, 0.f };
		line.fromPosition.x += aCameraPosition.x;
		line.toPosition.x += aCameraPosition.x;
		lineDrawer.Draw(line);
	}
	// Y Axis
	{
		line.color = { 0.f,1.f,0.f,1.0f };
		line.fromPosition = { 0.f, 1000.f, 0.f };
		line.toPosition = { 0.f, -1000.f, -0.f };
		line.fromPosition.y += aCameraPosition.y;
		line.toPosition.y += aCameraPosition.y;
		lineDrawer.Draw(line);
	}
	// Z Axis
	{
		line.color = { 0.f,0.f,1.f,1.0f };
		line.fromPosition = { 0.f, 0.f, 1000.f };
		line.toPosition = { 0.f, 0.f, -1000.f };
		line.fromPosition.z += aCameraPosition.z;
		line.toPosition.z += aCameraPosition.z;
		lineDrawer.Draw(line);
	}

	line.color = { 1.f,1.f,1.f,0.4f };
	// Render the Grid
	for (int x = 0; x <= myGridSize; x++)
	{
		for (int z = 0; z <= myGridSize; z++)
		{
			// First line 
			line.fromPosition = { myCellSize * x, -0.01f, 0 }; // Start point
			line.fromPosition += offset;
			line.toPosition = { myCellSize * x ,-0.01f,myCellSize * myGridSize }; // End point
			line.toPosition += offset;
			lineDrawer.Draw(line);

			// Second line 
			line.fromPosition = { 0, -0.01f, myCellSize * z }; // Start point
			line.fromPosition += offset;
			line.toPosition = { myCellSize * myGridSize ,-0.01f,myCellSize * z }; // End point
			line.toPosition += offset;
			lineDrawer.Draw(line);
		}
	}


}
