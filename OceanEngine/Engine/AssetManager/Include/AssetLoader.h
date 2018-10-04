#pragma once
#include <string>
#include <vector>
#include "AssetManager.h"
#include <Asset.h>

class BaseAssetLoader
{
public:
	BaseAssetLoader() = default;
	virtual void LoadFile(AssetManager& manager, std::string) = 0;

protected:
	template<typename T>
	void LoadAsset(AssetLoadEvent<T> Asset)
	{
		//check that the asset type name matches the type

		//Add to the assets vector
	}

private:
	std::vector<BaseAssetLoadEvent> Assets;
};