#include "stdafx.h"
#include <GameWorld.h>
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
	
	winconf.myApplicationName = L"TGE - Amazing Game";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	//winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings | Tga::DebugFeature::MemoryTrackingStackTraces | Tga::DebugFeature::MemoryTrackingAllAllocations;
	winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start(winconf))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}
	
	{
		GameWorld gameWorld;
		gameWorld.Init();

		Tga::Engine& engine = *Tga::Engine::GetInstance();

		while (engine.BeginFrame()) {
			gameWorld.Update(engine.GetDeltaTime());
			gameWorld.Render();

			engine.EndFrame();
		}
	}

	Tga::Engine::GetInstance()->Shutdown();
}

