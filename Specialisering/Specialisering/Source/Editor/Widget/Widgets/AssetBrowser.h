#pragma once
#include "../Widget.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Tga
{
	struct Asset;
	class Texture;
}

namespace Tga
{
	class AssetBrowser : public Widget
	{
	public:
		AssetBrowser(int aUniqueID);
		virtual ~AssetBrowser();

		bool Update(EditorContext& aContext) override;

	private:
		Tga::Asset* CreateAsset(fs::directory_entry aEntry);
	private:
		fs::path myCurrentDirectory;

		Texture* myFolderTexture = nullptr;
		Texture* myDocumentTexture = nullptr;
		Texture* myUnknownTexture = nullptr;
	};
}