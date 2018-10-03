#pragma once
#include <string>
#include "AssetManager.h"

class BaseAssetLoader
{
public:
	BaseAssetLoader() = default;
	virtual void LoadFile(AssetManager& manager, std::string) = 0;
};