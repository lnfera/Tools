#include <stdafx.h>
#include "Input.h"

bool Tga::InputManager::IsKeyDown(const int aKeyCode)
{
    return myKeyDown[aKeyCode];
}

bool Tga::InputManager::WasKeyReleased(const int aKeyCode)
{
	if (myKeyLastState[aKeyCode] == true && myKeyDown[aKeyCode] == false)
    {
        return true;
    }
    return false;
}

bool Tga::InputManager::WasKeyPressed(const int aKeyCode)
{

	if (myKeyLastState[aKeyCode] == false && myKeyDown[aKeyCode] == true)
	{
        return true;
    }
    return false;
}

Tga::Vector2f Tga::InputManager::GetMousePosition()
{
    return myMousePosition;
}

Tga::Vector2f Tga::InputManager::GetMouseDelta()
{
    return myMousePosition - myMouseLastPosition;
}

void Tga::InputManager::SetCursorPosition(Tga::Vector2f aPoint)
{
	SetCursorPos((int)aPoint.x, (int)aPoint.y);
}

void Tga::InputManager::LockCursor(HWND aWindow)
{
    RECT rect;
    GetClientRect(aWindow, &rect);

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(aWindow, nullptr, &ul, 1);
    MapWindowPoints(aWindow, nullptr, &lr, 1);

    rect.left = ul.x;
    rect.top = ul.y;

    rect.right = lr.x;
    rect.bottom = lr.y;

    ClipCursor(&rect);
}

void Tga::InputManager::UnlockCursor()
{
    ClipCursor(nullptr);
}

void Tga::InputManager::CursorSetVisability(bool aValue)
{
    ShowCursor(aValue);
}

int Tga::InputManager::GetDeltaScroll()
{
    return myDeltaScroll;
}

bool Tga::InputManager::UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM /*aLParam*/)
{
    switch (aMessage)
    {
	case WM_SYSKEYDOWN:
		myKeyDown.set(aWParam, true);
		return true;
	case WM_SYSKEYUP:
		myKeyDown.set(aWParam, false);
		return true;
	case WM_KEYUP:
		myKeyDown.set(aWParam, false);
		return true;
	case WM_KEYDOWN:
		myKeyDown.set(aWParam, true);
		return true;
		//Mouse Inputs
	case WM_MOUSEMOVE:
	{
		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(GetActiveWindow(), &mouse);
		myMousePosition.x = (float)mouse.x;
		myMousePosition.y = (float)mouse.y;

	}
		return true;
	case WM_MOUSEWHEEL:
		myCurrentScroll = GET_WHEEL_DELTA_WPARAM(aWParam);
		return true;
	case WM_LBUTTONDOWN:
		myKeyDown[0x01] = true;
		return true;
	case WM_LBUTTONUP:
		myKeyDown[0x01] = false;
		return true;
	case WM_RBUTTONDOWN:
		myKeyDown[0x02] = true;
		return true;
	case WM_RBUTTONUP:
		myKeyDown[0x02] = false;
		return true;
	case WM_MBUTTONDOWN:
		myKeyDown[0x04] = true;
		return true;
	case WM_MBUTTONUP:
		myKeyDown[0x04] = false;
		return true;
	}
	return false;
}

void Tga::InputManager::Update()
{
	//scroll
	myDeltaScroll = myCurrentScroll;
	myCurrentScroll = 0;
	//Mouse position update
	myMouseLastPosition = myMousePosition;
	//KeyStates update
	myKeyLastState = myKeyDown;
}
