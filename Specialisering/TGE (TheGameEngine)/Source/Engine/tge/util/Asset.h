#pragma once
#include <filesystem>
namespace fs = std::filesystem;

namespace Tga
{
	class Texture;
}

namespace Tga
{
	enum class eAssetType
	{
		undefined = 0,
		eFolder = 1,
		eTexture = 2,
		eDocument = 3,
		eUnknown = 4,
		eFBX = 5,
		COUNT
	};

	struct Asset
	{
		Texture* assetTexture;
		fs::path absolutePath;
		fs::path relativePath;
		eAssetType assetType = eAssetType::undefined;
	};

	static std::string path_with_forward_slashes(const std::filesystem::path& path) {
		std::string pathStr = path.string();
		for (char& ch : pathStr) {
			if (ch == '\\') {
				ch = '/';
			}
		}
		return pathStr;
	}

	static bool IsReadableTexture(std::string aPath)
	{
		if (aPath.find(".dds") != -1)
		{
			return true;
		}
		else if (aPath.find(".png") != -1)
		{
			return true;
		}
		else if (aPath.find(".jpg") != -1)
		{
			return true;
		}

		return false;
	}

	static bool IsFbxModel(std::string aPath)
	{
		if (aPath.find(".fbx") != -1)
		{
			return true;
		}

		return false;
	}

}