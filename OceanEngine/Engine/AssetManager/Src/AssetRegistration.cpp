#include "AssetRegistration.h"

namespace Assets
{

	//-----------------------------------------------------
	//					REQUEST ASSET
	//
	// 
	//----------------------------------------------------
	BaseAssetRequestEvent* RequestAsset(AssetManager* manager, std::experimental::filesystem::path assetPath)
	{
		detail::AssetRegistry& reg = detail::GetAssetRegistry();
		detail::AssetRegistry::iterator it = reg.find(assetPath.extension().string());

		if (it == reg.end())
			return nullptr;

		auto assetFunc = it->second;
		std::string assetType = std::get<0>(asset);
		detail::AssetFunction func = std::get<1>(asset);
		BaseAssetRequestEvent* request = nullptr;

		func(manager, request, assetPath.string(), assetType, detail::AssetActions::Retrieve);
		return request;
	}


	//--------------------------------------------------------------
	//					ADD ASSET
	//
	//------------------------------------------------------------
	void AddAsset(AssetManager* manager, BaseAssetRequestEvent* asset)
	{
		detail::AssetRegistry& reg = detail::GetAssetRegistry();
		detail::AssetRegistry::iterator it = reg.find(asset->GetAssetTypeName());

		if (it == reg.end())
			return;

		detail::AssetFunction assetFunc = it->second;
		assetFunc(manager, asset, asset->m_AssetName, asset->GetAssetTypeName(), detail::AssetActions::Add);
	}


	//-------------------------------------------------------
	//
	//
	//-------------------------------------------------------
	void LoadAssetFile(AssetManager& Manager, std::experimental::filesystem::path assetPath)
	{
		detail::FileRegistry& reg = detail::GetFileRegistry();
		detail::FileRegistry::iterator it = reg.find(assetPath.extension().string());

		if (it == reg.end())
			return;

		auto assetLoader = it->second;
		assetLoader->LoadFile(assetPath.string());

		
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