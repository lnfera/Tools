#pragma once
#include <Windows.h>
#include <bitset>
#include <memory>
namespace CommonUtilities
{
	class InputManager
	{
	public:
		//KeyBoardInputs
		inline static InputManager& GetInstance() 
		{
			static InputManager instance;
			return instance;
		};
		bool IsKeyPressed(const int aKeyCode);
		bool WasKeyPressed(const int aKeyCode);
		bool WasKeyReleased(const int aKeyCode);
		//MousePosition
		POINT GetMouseAbsolutePosition();
		POINT GetMousePosition();
		POINT GetMouseDeltaMove();
		void SetCursorPosition(POINT aPoint);
		//Lock Cursor within screen
		void LockCursorPosition(HWND aWindow);
		void UnlockCursorPosition();
		//ShowHideCursor
		void CursorSetActive(bool aValue);
		//ScrollWheel
		int GetDeltaScroll();
		//UpdateAllVariables
		bool UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);
		void Update();
	private:
		std::bitset<256> myKeyDown = false;
		std::bitset<256> myKeyLastState = false;
		POINT myMousePosition;
		POINT myLastMousePos;
		int myCurrentScroll;
		int myDeltaScroll;
	};
}
namespace CU = CommonUtilities;
