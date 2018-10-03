#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <imgui.h>
#include <EventManager.h>

#include <thread>
#include <string>
#include <map>
#include <functional>
#include <filesystem>

#include "AssetEvent.h"
#include "AssetPool.h"


//An AssetFile can hold reference to more than one asset or asset type. it should be used for
//files that contain more than one asset
class AssetFile
{
public:

	std::vector<std::string> m_AssetNames;
};


//-----------------------------------------------------------------------
//						ASSET MANAGER
//
//-----------------------------------------------------------------------
class AssetManager
{
public:
	
	AssetManager() = default;
	AssetManager(const std::string &a_assetDirectory);
	AssetManager(const std::string &a_assetDirectory, EventManager& eManager);

	template<typename T>
	void AddResourceType(std::string typeName);

	template<typename T>
	void AddAsset(std::string assetType, T asset, std::experimental::filesystem::path assetPath);

	void AddResource(std::string AssetType, std::experimental::filesystem::path assetPath);
	void DisplayAssetDirectory();
	void DisplayDirectoryContents();

	bool HandleAssetRequestEvent(BaseAssetRequestEvent& a_request);
	void RetrieveAsset(std::string assetType, std::string assetName);

	void LoadAllAssets();

private:
	void DisplayDirectoryRecursive(const std::experimental::filesystem::path& a_pathToShow, int a_level);
	void LoadAssetsRecursive(const std::experimental::filesystem::path& currentPath);

	std::thread m_WorkerThread;
	std::map<std::string, BaseAssetPool*> m_PoolMap;
	std::map<std::string, AssetFile> m_FileMap;

	bool IsWindowActive = false;
	std::experimental::filesystem::path m_AssetDirectory;
	std::experimental::filesystem::path m_SelectedPath;
};


//------------------------------------------------------------
//				TEMPLATE IMPLEMENTATIONS
//-----------------------------------------------------------


template <typename T>
void AssetManager::AddResourceType(std::string typeName)
{
	//Check that the pool dosen't already exist
	auto iterator = m_PoolMap.find(typeName);
	if (iterator == m_PoolMap.end())
	{
		BaseAssetPool* pool = new AssetPool<T>();
		//m_PoolMap.insert(std::map<std::string, BaseAssetPool*>::value_type(typeName, pool));
		m_PoolMap.insert(std::make_pair(typeName, pool));
	}
}


template<typename T>
void AssetManager::AddAsset(std::string assetType, T asset, std::experimental::filesystem::path assetPath)
{
	auto iterator = m_PoolMap.find(assetType);
	if (iterator != m_PoolMap.end())
	{
		AssetPool<T>* poolPtr = dynamic_cast<AssetPool<T>*>(iterator->second);
		if (poolPtr)
		{
			poolPtr->AddAsset(assetPath, asset);
		}
		else
		{
			std::cout<< "\n ASSET LOAD ERROR: The Asset typen name does not match the type of the object you passed."
		}

	}
}

#endif
