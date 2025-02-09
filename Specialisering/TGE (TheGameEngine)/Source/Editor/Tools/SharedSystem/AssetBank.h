#pragma once
#include <tge/util/Asset.h>
#include <unordered_map>

namespace Tga
{
	class AssetBank
	{
	public:
		inline static AssetBank& GetInstance()
		{
			static AssetBank instance;
			return instance;
		}

		void ShutDown()
		{
			for (auto asset : myAssets)
			{
				delete asset.second;
			}
			myAssets.clear();
		}

		~AssetBank() = default;

		std::unordered_map<fs::path, Tga::Asset*>& GetAssets() { return myAssets; };
	private:
		std::unordered_map<fs::path, Tga::Asset*> myAssets;


		AssetBank(AssetBank&) = delete;
		AssetBank& operator=(const AssetBank&) = delete;
		AssetBank() = default;

	};
}

#define ASSET_BANK() Tga::AssetBank::GetInstance()