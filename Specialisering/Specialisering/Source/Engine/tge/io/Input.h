#pragma once
#include <Windows.h>
#include <bitset>
#include <memory>
#include <tge/math/Vector.h>
namespace Tga
{
	class InputManager
	{
	public:
		//KeyBoard Input
		bool IsKeyDown(const int aKeyCode);
		bool WasKeyReleased(const int aKeyCode);
		bool WasKeyPressed(const int aKeyCode);

		//Mouse
		Tga::Vector2f GetMousePosition();
		Tga::Vector2f GetMouseDelta();
		void SetCursorPosition(Tga::Vector2f aPoint);
		//Lock Cursor within scTga::reen
		void LockCursor(HWND aWindow);
		void UnlockCursor();
		//ShowHideCursor
		void CursorSetVisability(bool aValue);
		//ScrollWheel
		int GetDeltaScroll();
		//UpdateAllVariables
		bool UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);
		void Update();

	private:
		std::bitset<256> myKeyDown = false;
		std::bitset<256> myKeyLastState = false;

		Tga::Vector2f myMousePosition;
		Tga::Vector2f myMouseLastPosition;

		int myCurrentScroll = 0;
		int myDeltaScroll = 0;
	};
}
