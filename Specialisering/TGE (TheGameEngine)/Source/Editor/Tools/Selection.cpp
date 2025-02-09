#include "Selection.h"
#include "Selection.h"
//static GameObject* mySelectedObject = nullptr;
static std::vector<Tga::GameObject*> gSelection;
static bool gHasBeenUpdated = false;

std::vector<Tga::GameObject*> Tga::Selection::GetSelection()
{
	return gSelection;
}

void Tga::Selection::SetSelection(std::vector<GameObject*> aList)
{
	gHasBeenUpdated = true;
	gSelection.clear();
	gSelection = aList;
}

void Tga::Selection::ClearSelection()
{
	gHasBeenUpdated = true;
	gSelection.clear();
}

void Tga::Selection::AddToSelection(GameObject* aObject)
{
	//Check if its already on the selection list
	gHasBeenUpdated = true;
	if (std::find(gSelection.begin(), gSelection.end(), aObject) == gSelection.end())
	{
		gSelection.push_back(aObject);
	}
}

void Tga::Selection::RemoveFromSelection(GameObject* aObject)
{
	gHasBeenUpdated = true;
	gSelection.erase(std::remove(gSelection.begin(), gSelection.end(), aObject), gSelection.end());
}

bool Tga::Selection::Contains(Tga::GameObject* aObject)
{
	return std::find(gSelection.begin(), gSelection.end(), aObject) != gSelection.end();
}

void Tga::Selection::ToggleSelect(GameObject* aObject)
{
	gHasBeenUpdated = true;
	if (std::find(gSelection.begin(), gSelection.end(), aObject) != gSelection.end()) {
		RemoveFromSelection(aObject);
	}
	else {
		AddToSelection(aObject);
	}
}
