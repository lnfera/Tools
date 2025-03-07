#include "stdafx.h"

#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/debugging/MemoryTracker.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/error/ErrorManager.h>
#include <tge/filewatcher/FileWatcher.h>
#include <tge/graphics/dx11.h>
#include <tge/text/TextService.h>
#include <tge/texture/TextureManager.h>
#include <tge/model/ModelFactory.h>
#include <tge/windows/WindowsWindow.h>
#include <tge/settings/settings.h>
#include <tge/io/Input.h>

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>
#include <tge/ImGui/ImGuiInterface.h>

#pragma comment( lib, "user32.lib" )


using namespace Tga; 
Engine* Tga::Engine::myInstance = nullptr;
Engine::Engine()
: myWindow(nullptr)
, myGraphicsEngine(nullptr)
, myTextureManager(nullptr)
, myDebugDrawer(nullptr)
, myRunEngine(true)
, myTotalTime(0.0f)
, myDeltaTime(0.0f)
, myShouldExit(false)
, myWantToUpdateSize(false)
{
	{ // if specified for incomming parameters, prioritize
		myWindowConfiguration = Settings::GetEngineConfiguration();
		EngineConfiguration &cfg = myWindowConfiguration;
		{
			// if fullscreen we should not care what the settings say for the window size..
			if (cfg.myStartInFullScreen || cfg.myStartMaximized) {
				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				int screenHeight = GetSystemMetrics(SM_CYSCREEN);
				cfg.myWindowSize = cfg.myRenderSize = cfg.myTargetSize = { (uint32_t)screenWidth, (uint32_t)screenHeight };
			}
			else {
				if (cfg.myTargetSize.x <= 0 && cfg.myTargetSize.y <= 0)
				{
					cfg.myTargetSize = cfg.myRenderSize;
				}
			}
		}

		myDebugDrawer = std::make_unique<DebugDrawer>(myWindowConfiguration.myActivateDebugSystems != DebugFeature::None);
		myWindowConfiguration.myHwnd = cfg.myHwnd;
		myWindowConfiguration.myHInstance = cfg.myHInstance;
	}

	myErrorManager = std::make_unique<ErrorManager>();
	myErrorManager->AddLogListener(myWindowConfiguration.myLogFunction, myWindowConfiguration.myErrorFunction);
}


Engine::~Engine()
{}


void Tga::Engine::DestroyInstance()
{
	if (myInstance)
	{
		delete myInstance;
		myInstance = nullptr;
		StopMemoryTrackingAndPrint();
	}
}

bool Engine::Start()
{
	if (!myInstance)
	{
		EngineConfiguration cfg = Settings::GetEngineConfiguration();

		MemoryTrackingSettings trackingSettings;
		trackingSettings.myShouldTrackAllAllocations = ((cfg.myActivateDebugSystems & DebugFeature::MemoryTrackingAllAllocations) != DebugFeature::None);
		trackingSettings.myShouldStoreStackTraces = ((cfg.myActivateDebugSystems & DebugFeature::MemoryTrackingStackTraces) != DebugFeature::None);
		StartMemoryTracking(trackingSettings);

		myInstance = new Engine();
		return myInstance->InternalStart();
	}
	else
	{
		ERROR_PRINT("%s", "DX2D::Engine::CreateInstance called twice, thats bad.");
	}
	return false;
}

bool Engine::InternalStart()
{
	INFO_PRINT("%s", "#########################################");
	INFO_PRINT("%s", "---TGA 2D Starting, dream big and dare to fail---");
	myFileWatcher = std::make_unique<FileWatcher>();
	myWindow = std::make_unique<WindowsWindow>();
	if (!myWindow->Init(myWindowConfiguration, myWindowConfiguration.myHInstance, myWindowConfiguration.myHwnd)) 
	{
		ERROR_PRINT("%s", "Window failed to be created!");
		return false;
	}

	myDx11 = std::make_unique<DX11>();
	if (!myDx11->Init(myWindow.get()))
	{
		ERROR_PRINT("%s", "D3D failed to be created!");
		myWindow->Close();
		return false;
	}

	myGraphicsEngine = std::make_unique<GraphicsEngine>();
	if (!myGraphicsEngine->Init())
	{
		ERROR_PRINT("%s", "D3D engine failed to be created!");
		myWindow->Close();
		return false;
	}

	myTextureManager = std::make_unique<TextureManager>();
	myTextureManager->Init();

	myTextService = std::make_unique<TextService>();
	myTextService->Init();

	myInputManager = std::make_unique<InputManager>();

	CalculateRatios();

	if (myDebugDrawer)
	{
		myDebugDrawer->Init();
	}
#ifndef _RETAIL
	ImGuiInterface::Init();
#endif // !_RETAIL

	myStartOfTime = std::chrono::steady_clock::now();



	return true;
}

void Tga::Engine::Shutdown()
{
	ModelFactory::DestroyInstance();
	ImGuiInterface::Shutdown();

	if (myInstance)
	{
		DestroyInstance();
	}
}

void GetDesktopResolution(int& horizontal, int& vertical, HWND aHwnd)
{
	RECT r;
	GetClientRect(aHwnd, &r); //get window rect of control relative to screen
	horizontal = r.right - r.left;
	vertical = r.bottom - r.top;
}

void Tga::Engine::UpdateWindowSizeChanges(bool aIgnoreAutoUpdate)
{
	if (myWindowConfiguration.myUseLetterboxAndPillarbox || aIgnoreAutoUpdate)
	{
		int horizontal = 0;
		int vertical = 0;
		GetDesktopResolution(horizontal, vertical, *myWindowConfiguration.myHwnd);

		// Both these values must be your real window size, so of course these values can't be static
		float screen_width = (float)horizontal;
		float screen_height = (float)vertical;

		// This is your target virtual resolution for the game, the size you built your game to
		auto& targetsize = myWindowConfiguration.myTargetSize;
		float virtual_width = (float)targetsize.x;
		float  virtual_height = (float)targetsize.y;

		float targetAspectRatio = virtual_width / virtual_height;

		// figure out the largest area that fits in this resolution at the desired aspect ratio
		float  width = screen_width;
		float  height = (float)(width / targetAspectRatio);

		if (height > screen_height)
		{
			//It doesn't fit our height, we must switch to pillarbox then
			height = screen_height;
			width = (float)(height * targetAspectRatio);
		}

		// set up the new viewport centered in the backbuffer
		float  vp_x = (screen_width / 2.0f) - (width / 2.0f);
		float  vp_y = (screen_height / 2.0f) - (height / 2.0f);

		static float lastWidth = 0.0f;
		static float lastHeight = 0.0f;

		{
			if (myWindowConfiguration.myTargetSize.x != width || myWindowConfiguration.myTargetSize.y != height)
			{
				INFO_TIP("%s", "Warning: Window is resized, it is not matching your native target resolution, there might be flickering on sprites that are close to one another");
			}

			lastWidth = width;
			lastHeight = height;
			SetViewPort(vp_x, vp_y, width, height, 0, 1, true);
		}
		
	}
	else
	{
		int horizontal = 0;
		int vertical = 0;
		GetDesktopResolution(horizontal, vertical, *myWindowConfiguration.myHwnd);
		Vector2ui size = Vector2ui(horizontal, vertical);		
		
		SetResolution(size, false);
		SetViewPort(
			0, 0, 
			static_cast<float>(size.x),
			static_cast<float>(size.y)
		);
	}
}

float Tga::Engine::GetWindowRatio() const
{
	return myWindowRatio;
}

float Tga::Engine::GetWindowRatioInversed() const
{
	return myWindowRatioInversed;
}

Vector2f Tga::Engine::GetWindowRatioVec() const
{
	return myWindowRatioVec;
}

Vector2f Tga::Engine::GetWindowRatioInversedVec() const
{
	return myWindowRatioInversedVec;
}

void Tga::Engine::SetTargetSize(const Vector2ui& aTargetSize)
{
	myWindowConfiguration.myTargetSize = aTargetSize;
}

void Tga::Engine::SetResolution(const Vector2ui &aResolution, bool aAlsoSetWindowSize)
{
	myWindowConfiguration.myWindowSize = aResolution;
	if (aAlsoSetWindowSize)
	{
		myWindow->SetResolution(aResolution);
	}
	myDx11->Resize(aResolution);
	DX11::BackBuffer->SetAsActiveTarget();

	CalculateRatios();
}

void Tga::Engine::CalculateRatios()
{
	float sizeX = static_cast<float>(myWindowConfiguration.myWindowSize.x);
	float sizeY = static_cast<float>(myWindowConfiguration.myWindowSize.y);
	if (sizeY > sizeX)
	{
		float temp = sizeX;
		sizeX = sizeY;
		sizeY = temp;
	}

	myWindowRatio = static_cast<float>(sizeX) / static_cast<float>(sizeY);
	myWindowRatioInversed = static_cast<float>(sizeY) / static_cast<float>(sizeX);
	
	myWindowRatioVec.x = 1.0f;
	myWindowRatioVec.y = 1.0f;
	myWindowRatioInversedVec.x = 1.0f;
	myWindowRatioInversedVec.y = 1.0f;
	if (sizeX >= sizeY)
	{
		myWindowRatioVec.y = myWindowRatio;
		myWindowRatioInversedVec.y = myWindowRatioInversed;
	}
	else
	{
		myWindowRatioVec.x = myWindowRatio;
		myWindowRatioInversedVec.x = myWindowRatioInversed;
	}
}

HWND* Tga::Engine::GetHWND() const
{
	return myWindowConfiguration.myHwnd;
}


HINSTANCE Tga::Engine::GetHInstance() const
{
	return myWindowConfiguration.myHInstance;
}

void Tga::Engine::SetViewPort(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth, float aMaxDepth, bool aSetEngineResolution)
{
	if (myGraphicsEngine)
	{
		if (aSetEngineResolution)
		{
			SetResolution(Vector2ui(static_cast<unsigned int>(aWidth), static_cast<unsigned int>(aHeight)), false);
		}	

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = aTopLeftX;
		viewport.TopLeftY = aTopLeftY;
		viewport.Width = aWidth;
		viewport.Height = aHeight;
		viewport.MinDepth = aMinDepth;
		viewport.MaxDepth = aMaxDepth;

		DX11::Context->RSSetViewports(1, &viewport);
	}
}

void Tga::Engine::SetClearColor(const Color& aClearColor)
{
	myWindowConfiguration.myClearColor = aClearColor;
}

void Tga::Engine::SetFullScreen(bool aFullScreen)
{
	if (myGraphicsEngine)
	{
		myGraphicsEngine->SetFullScreen(aFullScreen);
	}
}

bool Tga::Engine::IsDebugFeatureOn(DebugFeature aFeature) const
{
	const bool all = ((myWindowConfiguration.myActivateDebugSystems & DebugFeature::All) != DebugFeature::None);
	if (all)
	{
		return true;
	}

	const bool specific = ((myWindowConfiguration.myActivateDebugSystems & aFeature) != DebugFeature::None);
	return specific;
}

bool Engine::BeginFrame()
{
	if (myShouldExit)
	{
		return false;
	}

	MSG msg = { 0 };

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			INFO_PRINT("%s", "Exiting...");
			myShouldExit = true;
			return false;
		}
	}
#ifndef _RETAIL
	ImGuiInterface::PreFrame();
#endif // !_RETAIL
    myFileWatcher->FlushChanges();
	
	myDx11->BeginFrame(myWindowConfiguration.myClearColor);
    myTextureManager->Update();
	
	myGraphicsEngine->GetGraphicsStateStack().BeginFrame();

	DX11::ResetDrawCallCounter();

	return true;
}


void Engine::EndFrame( void )
{
#ifndef _RETAIL
	DX11::BackBufferNoSrgbConversion->SetAsActiveTarget();

	ImGuiInterface::Render();

	DX11::BackBuffer->SetAsActiveTarget();

#endif // !_RETAIL

	myTimer.Tick([&]()
	{
		myDeltaTime = static_cast<float>(myTimer.GetElapsedSeconds());
		myTotalTime += static_cast<float>(myTimer.GetElapsedSeconds());
	});
	if (myDebugDrawer)
	{
		myDebugDrawer->Update(myDeltaTime);
		myDebugDrawer->Render();
	}

	myDx11->EndFrame(myWindowConfiguration.myEnableVSync);

	if (myWantToUpdateSize)
	{
		UpdateWindowSizeChanges();
		myWantToUpdateSize = false;
	}
}
