#include <stdafx.h>
#include "settings.h"

#include <filesystem>

#include <nlohmann/json.hpp>
#include <fstream>

#include <tge/util/StringCast.h>
#include <tge/stringRegistry/StringRegistry.h>

namespace fs = std::filesystem;

namespace Tga 
{
	namespace Settings 
	{
		static fs::path engine_assets_path;
		static fs::path game_assets_path;
		static nlohmann::json game_settings;

		static EngineConfiguration window_params;
	}
	namespace Preferences
	{
		bool loadLastProject;
	}
}


std::string Tga::Settings::Window::title;


const fs::path& Tga::Settings::EngineAssetRoot() {
	return Tga::Settings::engine_assets_path;
}
const fs::path& Tga::Settings::GameAssetRoot() {
	return Tga::Settings::game_assets_path;
}

Tga::EngineConfiguration& Tga::Settings::GetEngineConfiguration() 
{
	return window_params;
}

std::string_view Tga::Settings::ResolveAssetPath(const std::string& anAsset) 
{
	StringId assetId = Tga::StringRegistry::RegisterOrGetString(anAsset);
	std::string_view assetView = assetId.GetString();

	if (fs::exists(assetView)) {
		return assetView;
	}
	else if (fs::exists("data/" + anAsset)) {
		assetId = StringRegistry::RegisterOrGetString("data/" + anAsset);
		return assetId.GetString();
	}

	else if (fs::exists(game_assets_path / anAsset)) {
		assetId = StringRegistry::RegisterOrGetString((game_assets_path / anAsset).string());
		return assetId.GetString();
	}
	else if (fs::exists(engine_assets_path / anAsset)) {
		assetId = StringRegistry::RegisterOrGetString((engine_assets_path / anAsset).string());
		return assetId.GetString();
	}
	return "";
}

std::wstring Tga::Settings::ResolveAssetPathW(const std::string& anAsset) {
	StringId assetId = Tga::StringRegistry::RegisterOrGetString(anAsset);
	std::string_view assetView = assetId.GetString();

	if (fs::exists(assetView)) {
		return string_cast<std::wstring>(anAsset);
	}
	else if (fs::exists("data/" + anAsset)) {
		return (L"data/" + string_cast<std::wstring>(anAsset));
	}
	else if (fs::exists(game_assets_path / anAsset)) {
		return string_cast<std::wstring>(game_assets_path / anAsset);
	}
	else if (fs::exists(engine_assets_path / anAsset)) {
		return string_cast<std::wstring>(engine_assets_path / anAsset);
	}
	return L"";
}

std::wstring Tga::Settings::ResolveAssetPathW(const std::wstring& anAsset) {
	std::string asset = string_cast<std::string>(anAsset);
	
	if (fs::exists(asset)) {
		return anAsset;
	}
	else if (fs::exists("data/" + asset)) {
		return (L"data/" + anAsset);
	}
	else if (fs::exists(game_assets_path / asset)) {
		return string_cast<std::wstring>((game_assets_path / asset).string());
	}
	else if (fs::exists(engine_assets_path / asset)) {
		return string_cast<std::wstring>((engine_assets_path / asset).string());
	}
	return L"";
}

std::string Tga::Settings::ResolveEngineAssetPath(const std::string& anAsset) {
	if (fs::exists(anAsset)) {
		return anAsset;
	}
	if (fs::exists(engine_assets_path / anAsset)) {
		return (engine_assets_path / anAsset).string();
	}
	return "";
}

std::wstring Tga::Settings::ResolveEngineAssetPathW(const std::string& anAsset) {
	if (fs::exists(anAsset)) {
		return string_cast<std::wstring>(anAsset);
	}
	if (fs::exists(engine_assets_path / anAsset)) {
		return string_cast<std::wstring>(engine_assets_path / anAsset);
	}
	return L"";
}

std::wstring Tga::Settings::ResolveEngineAssetPathW(const std::wstring& anAsset) {
	if (fs::exists(anAsset)) {
		return anAsset;
	}
	if (fs::exists(game_assets_path / anAsset)) {
		return (engine_assets_path / anAsset);
	}
	return L"";
}

std::string Tga::Settings::ResolveGameAssetPath(const std::string& anAsset) {
	if (fs::exists(anAsset)) {
		return anAsset;
	}
	if (fs::exists(game_assets_path / anAsset)) {
		return (game_assets_path / anAsset).string();
	}
	return "";
}

std::wstring Tga::Settings::ResolveGameAssetPathW(const std::string& anAsset) {
	if (fs::exists(anAsset)) {
		return string_cast<std::wstring>(anAsset);
	}
	if (fs::exists(game_assets_path / anAsset)) {
		return (game_assets_path / anAsset);
	}
	return L"";
}
std::wstring Tga::Settings::ResolveGameAssetPathW(const std::wstring& anAsset) 
{
	if (fs::exists(anAsset)) {
		return anAsset;
	}
	if (fs::exists(game_assets_path / anAsset)) {
		return (game_assets_path / anAsset);
	}
	return L"";
}

std::string Tga::Settings::ResolveProjectPath(const char* aRelativePath) {
	fs::path dir = aRelativePath;

	if (fs::exists(dir)) {
		return dir.string();
	}

	return fs::absolute(dir).string();
}

nlohmann::json &Tga::Settings::GetGameSettings()
{
	return game_settings;
}

//std::string Tga::Settings::GameSettings(const std::string& aKey) {
//	return game_settings[aKey];
//}
//std::wstring Tga::Settings::GameSettingsW(const std::string& aKey) {
//	std::string s = game_settings[aKey];
//	return string_cast<std::wstring>(game_settings[aKey]);
//}
//std::wstring Tga::Settings::GameSettingsW(const std::wstring& aKey) {
//	std::string s = game_settings[string_cast<std::string>(aKey)];
//	return string_cast<std::wstring>(s);
//}

void Tga::SaveSettings(const std::string& aProjectName) {
	using namespace Settings;
	{
		using namespace nlohmann;

		std::string filename = (aProjectName.find(".") == std::string::npos) ? (aProjectName + ".json").c_str() : aProjectName.c_str();
		std::string path("settings/");

		//Settings::game_settings["assets_path"]["editor"]["project"]		= editor_project_assets_path.string();
		Settings::game_settings["assets_path"]["engine"]	= engine_assets_path;
		Settings::game_settings["assets_path"]["game"]		= game_assets_path;

		std::ofstream game_out(path + filename);
		assert(game_out);
		game_out << Settings::game_settings;
		game_out.close();
	}
}

void Tga::LoadSettings(const std::string& aProjectName) {
	using namespace Settings;
	{	
		using namespace nlohmann;
					
		/////////////////////
		// Game settings
		{
			std::string filename = (aProjectName.find(".") == std::string::npos) ? (aProjectName + ".json").c_str() : aProjectName.c_str();
			std::string path("settings/");

			std::ifstream game_ifs(path + filename);
			assert(game_ifs);

			game_ifs >> Settings::game_settings;
			game_ifs.close();
		}

		if (game_settings.contains("assets_path")) {
			std::string enginePath = game_settings["assets_path"]["engine"]["absolute"];
			std::string gamePath = game_settings["assets_path"]["game"]["absolute"];
			engine_assets_path = enginePath;
			game_assets_path = gamePath;
		}

		//////////////////////////////////////
		// Window Title
		{
			std::string app		= game_settings["window_settings"]["title"];
			if (!app.empty()) {
				Settings::window_params.myApplicationName = string_cast<std::wstring>(app);
			}
		}
		/////////////////////////////////////
		// Clear Color
		{
			auto& app		= game_settings["window_settings"]["clear_color"];
			if (!app.is_null()) {
				Settings::window_params.myClearColor = Tga::Color(app["r"], app["g"], app["b"], app["a"]);
			}
		}
		/////////////////////////////////////
		// Window Width & Height
		{
			auto& window = game_settings["window_settings"]["window_size"];
			auto& render = game_settings["window_settings"]["render_size"];
			auto& target = game_settings["window_settings"]["target_size"];

			if (window.is_null() == false) {
				Settings::window_params.myWindowSize.x = window["w"];
				Settings::window_params.myWindowSize.y = window["h"];
				if (render.is_null()) {
					Settings::window_params.myRenderSize.x = window["w"];
					Settings::window_params.myRenderSize.y = window["h"];
				}
				else {
					Settings::window_params.myRenderSize.x = render["w"];
					Settings::window_params.myRenderSize.y = render["h"];
				}
			}
			else if (render.is_null() == false) {
				Settings::window_params.myRenderSize.x = render["w"];
				Settings::window_params.myRenderSize.y = render["h"];
				Settings::window_params.myWindowSize.x = render["w"];
				Settings::window_params.myWindowSize.y = render["h"];
			}
		
			if (target.is_null() == false) {
				Settings::window_params.myTargetSize.x = target["w"];
				Settings::window_params.myTargetSize.y = target["h"];
			}
			else {
				Settings::window_params.myTargetSize.x = 0;
				Settings::window_params.myTargetSize.y = 0;
			}
		}
		//////////////////////////////////////
		// VSync
		{
			auto& app = game_settings["enable_vsync"];
			if (!app.is_null()) {
				Settings::window_params.myEnableVSync = app;
			}
		}
		/////////////////////////////////////
		// Start in fullscreen / Maximized
		{
			auto& app = game_settings["window_settings"]["start_in_fullscreen"];
			if (!app.is_null()) {
				Settings::window_params.myStartInFullScreen = app;
			}
		}
		{
			auto& app = game_settings["window_settings"]["start_maximized"];
			if (!app.is_null()) {
				Settings::window_params.myStartMaximized = app;
			}
		}
		/////////////////////////////////////
		// Letterbox and pillarbox
		{
			auto& app = game_settings["window_settings"]["use_letterbox_and_pillarbox"];
			if (!app.is_null()) {
				Settings::window_params.myUseLetterboxAndPillarbox = app;
			}
		}

		/////////////////////////////////////
		// Window setting (overlapped/borderless)
		{
			auto& app = game_settings["window_settings"]["borderless"];
			if (!app.is_null()) {
				Settings::window_params.myBorderless = app;
			}
		}

		/////////////////////////////////////
		// Debug systems
		{
			auto& app = game_settings["debug_features"];
			DebugFeature dbg = static_cast<DebugFeature>(0);

			for (std::string flag : app) {
				if (flag == "All") { dbg = DebugFeature::All; break; }
				if (flag == "None") { dbg = DebugFeature::None; break; }
				if (flag == "Cpu") { dbg = dbg | DebugFeature::Cpu; continue; }
				if (flag == "Drawcalls") { dbg = dbg | DebugFeature::Drawcalls; continue; }
				if (flag == "Filewatcher") { dbg = dbg | DebugFeature::Filewatcher; continue; }
				if (flag == "Fps") { dbg = dbg | DebugFeature::Fps; continue; }
				if (flag == "FpsGraph") { dbg = dbg | DebugFeature::FpsGraph; continue; }
				if (flag == "Mem") { dbg = dbg | DebugFeature::Mem; continue; }
				if (flag == "MemoryTrackingAllAllocations") { dbg = dbg | DebugFeature::MemoryTrackingAllAllocations; continue; }
				if (flag == "MemoryTrackingStackTraces") { dbg = dbg | DebugFeature::MemoryTrackingStackTraces; continue; }
				if (flag == "OptimizeWarnings") { dbg = dbg | DebugFeature::OptimizeWarnings; continue; }
			}
			//if (dbg > static_cast<DebugFeature>(0))
			{
				Settings::window_params.myActivateDebugSystems = dbg;
			}
		}
		
		/////////////////////////////////////
		// Multisampling Quality
		{
			auto& app = game_settings["multisampling"];

			for (std::string flag : app) {
				if (flag == "Off") {
					Settings::window_params.myPreferedMultiSamplingQuality = MultiSamplingQuality::Off;
				}
				if (flag == "Low") {
					Settings::window_params.myPreferedMultiSamplingQuality = MultiSamplingQuality::Low;
				}
				if (flag == "Medium") {
					Settings::window_params.myPreferedMultiSamplingQuality = MultiSamplingQuality::Medium;
				}
				if (flag == "High") {
					Settings::window_params.myPreferedMultiSamplingQuality = MultiSamplingQuality::High;
				}
			}
		}
	}
}