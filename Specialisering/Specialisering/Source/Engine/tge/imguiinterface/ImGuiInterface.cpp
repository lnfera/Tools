#include "stdafx.h"
#include "ImGuiInterface.h"
#include <string>
#include <tge/engine.h>
#include <tge/graphics/DX11.h>
#include <imnodes/imnodes.h>


//#pragma comment(lib, "..\\Libs\\imgui.lib")
//#pragma comment(lib, "..\\Libs\\imgui_canvas.lib")

using namespace Tga;

namespace Tga
{
	class ImGuiInterfaceImpl
	{
	public:
		ImFontAtlas myFontAtlas;
	};
}

ImGuiInterface::ImGuiInterface() = default;

ImGuiInterface::~ImGuiInterface()
{
#ifndef _RETAIL
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
#endif
}

#ifndef _RETAIL
static ImFont* ImGui_LoadFont(ImFontAtlas& atlas, const char* name, float size)
{
	char* windir = nullptr;
	if (_dupenv_s(&windir, nullptr, "WINDIR") || windir == nullptr)
		return nullptr;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0104, 0x017C, // Polish characters and more
		0,
	};

	ImFontConfig config;
	config.OversampleH = 4;
	config.OversampleV = 4;
	config.PixelSnapH = false;

	auto path = std::string(windir) + "\\Fonts\\" + name;
	auto font = atlas.AddFontFromFileTTF(path.c_str(), size, &config, ranges);

	free(windir);

	return font;
}
#endif

void ImGuiInterface::Init()
{
#ifndef _RETAIL
	myImpl = std::make_unique<ImGuiInterfaceImpl>();

	ImGui_LoadFont(myImpl->myFontAtlas, "segoeui.ttf", 18.0f);//16.0f * 96.0f / 72.0f);
	myImpl->myFontAtlas.Build();

	ImGui::CreateContext(&myImpl->myFontAtlas);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	ImNodes::CreateContext();

	ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 2;

	// Setup ImGui binding
	ImGui_ImplWin32_Init(*Tga::Engine::GetInstance()->GetHWND());
	ImGui_ImplDX11_Init(Tga::DX11::Device, Tga::DX11::Context);
	ImGui::StyleColorsDark();

	ImVec4 darkest = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
	ImVec4 darker = ImVec4{ 0.13f, 0.13f, 0.13f, 1.0f };
	ImVec4 dark = ImVec4{ 0.16f, 0.16f, 0.16f, 1.0f };
	ImVec4 darkish = ImVec4{ 0.24f, 0.24f, 0.24f, 1.0f };

	ImVec4 light = ImVec4{ 0.5f, 0.1f, 0.1f, 1.f };
	ImVec4 lighter = ImVec4{ 0.8f, 0.1f, 0.1f, 1.f };

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_TextDisabled] = dark;

	colors[ImGuiCol_WindowBg] = darkest;
	colors[ImGuiCol_MenuBarBg] = dark;

	// Border
	colors[ImGuiCol_Border] = dark;
	colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

	// Text
	colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = light;
	colors[ImGuiCol_HeaderHovered] = lighter;
	colors[ImGuiCol_HeaderActive] = lighter;

	// Buttons
	colors[ImGuiCol_Button] = darker;
	colors[ImGuiCol_ButtonHovered] = light;
	colors[ImGuiCol_ButtonActive] = lighter;
	colors[ImGuiCol_CheckMark] = lighter;

	// Popups
	colors[ImGuiCol_PopupBg] = ImVec4{ darkest.x, darkest.y, darkest.z, 0.92f };

	// Slider
	colors[ImGuiCol_SliderGrab] = lighter;
	colors[ImGuiCol_SliderGrabActive] = lighter;

	// Frame BG
	colors[ImGuiCol_FrameBg] = darker;
	colors[ImGuiCol_FrameBgHovered] = light;
	colors[ImGuiCol_FrameBgActive] = dark;

	// Tabs
	colors[ImGuiCol_Tab] = dark;
	colors[ImGuiCol_TabHovered] = lighter;
	colors[ImGuiCol_TabSelected] = darkish;
	colors[ImGuiCol_TabSelectedOverline] = lighter;
	colors[ImGuiCol_TabUnfocused] = dark;
	colors[ImGuiCol_TabUnfocusedActive] = darkish;

	// Title
	colors[ImGuiCol_TitleBg] = darker;
	colors[ImGuiCol_TitleBgActive] = darker;
	colors[ImGuiCol_TitleBgCollapsed] = darker;

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = dark;
	colors[ImGuiCol_ScrollbarGrab] = light;
	colors[ImGuiCol_ScrollbarGrabHovered] = lighter;
	colors[ImGuiCol_ScrollbarGrabActive] = lighter;

	// Separator
	colors[ImGuiCol_Separator] = dark;
	colors[ImGuiCol_SeparatorHovered] = lighter;
	colors[ImGuiCol_SeparatorActive] = lighter;

	// Separator
	colors[ImGuiCol_TableBorderLight] = dark;
	colors[ImGuiCol_TableBorderStrong] = dark;

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = dark;
	colors[ImGuiCol_ResizeGripHovered] = lighter;
	colors[ImGuiCol_ResizeGripActive] = lighter;

	// Docking
	colors[ImGuiCol_DockingPreview] = dark;

	// Text
	colors[ImGuiCol_TextSelectedBg] = lighter;


#endif
}

void ImGuiInterface::PreFrame()
{
#ifndef _RETAIL
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImGuiInterface::Render()
{
#ifndef _RETAIL
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
#endif
}
