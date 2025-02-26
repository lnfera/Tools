#pragma once

#include <tge/engine.h>
#include <nlohmann/json.hpp>

#include <filesystem>
namespace fs = std::filesystem;

namespace Tga {
	extern void LoadSettings(const std::string &aProjectName);
	extern void SaveSettings(const std::string &aProjectName);

	namespace Preferences
	{
		extern bool loadLastProject;
	}
	
	namespace Settings {
		namespace Window {
			extern std::string title;
		}

		extern const fs::path& EngineAssetRoot();
		extern const fs::path& GameAssetRoot();

		extern EngineConfiguration &GetEngineConfiguration();

		extern std::string_view ResolveAssetPath(const std::string& anAsset);
		extern std::wstring ResolveAssetPathW(const std::string& anAsset);
		extern std::wstring ResolveAssetPathW(const std::wstring& anAsset);

		extern std::string ResolveEngineAssetPath(const std::string& anAsset);
		extern std::wstring ResolveEngineAssetPathW(const std::string& anAsset);
		extern std::wstring ResolveEngineAssetPathW(const std::wstring& anAsset);

		extern std::string ResolveGameAssetPath(const std::string& anAsset);
		extern std::wstring ResolveGameAssetPathW(const std::string& anAsset);
		extern std::wstring ResolveGameAssetPathW(const std::wstring& anAsset);

		//extern std::string GameSettings(const std::string& aKey);
		extern nlohmann::json& GetGameSettings();
		//extern std::wstring GameSettingsW(const std::string& aKey);
		//extern std::wstring GameSettingsW(const std::wstring& aKey);

		extern std::string ResolveProjectPath(const char* aRelativePath);
	}
}