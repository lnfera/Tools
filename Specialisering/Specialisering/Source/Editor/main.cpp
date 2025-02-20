#pragma once
#include "stdafx.h"
#include "Editor.h"
#include <tge/engine.h>
#include <tge/io/Input.h>
#include <Core/MainSingleton.h>
void Go(void);

int main(const int /*argc*/, const char* /*argc*/[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Go();
	return 0;
}

LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	hWnd;
	message;
	wParam;
	lParam;

	if (Tga::Engine::GetInstance()->GetInputManager().UpdateEvents(message, wParam, lParam))
	{
		return 0;
	}

	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	}

	return 0;
}


void Go()
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration winconf;

	winconf.myApplicationName = L"TGE - The Game Editor";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	//winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings | Tga::DebugFeature::MemoryTrackingStackTraces | Tga::DebugFeature::MemoryTrackingAllAllocations;
	//winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
	winconf.myActivateDebugSystems = Tga::DebugFeature::None;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start())
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}

	{

		Tga::MainSingleton& mSingleton = Tga::MainSingleton::GetInstance();
		mSingleton.Init();

		Tga::Editor editor;
		editor.Init();
		Tga::Engine& engine = *Tga::Engine::GetInstance();

		while (engine.BeginFrame()) 
		{
			editor.Update(engine.GetDeltaTime());

			engine.EndFrame();
		}

	}

	Tga::Engine::GetInstance()->Shutdown();
}

