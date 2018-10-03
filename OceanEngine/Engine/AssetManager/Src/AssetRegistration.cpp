#include "AssetRegistration.h"

namespace Assets
{
	BaseAssetRequestEvent* RequestAsset(AssetManager* manager, std::experimental::filesystem::path assetPath)
	{
		detail::AssetRegistry& reg = detail::GetAssetRegistry();
		detail::AssetRegistry::iterator it = reg.find(assetPath.extension().string());

		if (it == reg.end())
			return nullptr;

		auto asset = it->second;
		std::string assetType = std::get<0>(asset);
		detail::AssetFunction func = std::get<1>(asset);

		return func(manager, assetPath.string(), assetType, detail::AssetActions::Retrieve);

	}



	void LoadNewAsset(AssetManager* manager, std::experimental::filesystem::path assetPath)
	{
		detail::AssetRegistry& reg = detail::GetAssetRegistry();
		detail::AssetRegistry::iterator it = reg.find(assetPath.extension().string());

		if (it == reg.end())
			return;

		auto asset = it->second;
		std::string assetType = std::get<0>(asset);
		detail::AssetFunction func = std::get<1>(asset);

		func(manager, assetPath.string(), assetType, detail::AssetActions::Load);
	}

	void CheckAsset()
	{

	}
}