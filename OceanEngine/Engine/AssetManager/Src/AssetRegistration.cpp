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
		BaseAssetRequestEvent* request = nullptr;

		func(manager, request, assetPath.string(), assetType, detail::AssetActions::Retrieve);
		return request;
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

		func(manager, nullptr, assetPath.string(), assetType, detail::AssetActions::Load);
	}

	//bool CheckAsset()
	//{
	//	detail::AssetRegistry& reg = detail::GetAssetRegistry();
	//	detail::AssetRegistry::iterator it = reg.find(assetPath.extension().string());

	//	if (it == reg.end())
	//		return;

	//	auto asset = it->second;
	//	std::string assetType = std::get<0>(asset);
	//	detail::AssetFunction func = std::get<1>(asset);

	//	func(manager, assetPath.string(), assetType, detail::AssetActions::Load);
	//}
}