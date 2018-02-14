#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <imgui.h>
#include <EventManager.h>

#include <thread>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <functional>
#include <filesystem>

//-----------------------------------------------
//			AssetRequestEvent
//
// These events are used to request assets for components
//----------------------------------------------

class BaseAssetRequestEvent : public IEvent
{
public:
	std::string m_AssetTypeName;
};

template<typename T>
class AssetRequestEvent : public BaseAssetRequestEvent
{
public:
	std::string m_AssetName;
	T Asset;
};


//--------------------------------------------------
// BaseAssetPool
//--------------------------------------------------

class BaseAssetPool
{
public:
	virtual void LoadAsset(std::string a_filePath) = 0;
	virtual void RetrieveAsset(BaseAssetRequestEvent* request) = 0;
private:
	std::function<void(std::string)> m_Load;
	std::function<void()> m_UnLoad;
	std::string PoolName;
};

//---------------------------------------------------
//					Asset Pool
//
// Holds a vector of asset type T.
// Has Ability to Load, store and retrieve asset of type T.
// Is used in combination with the editor.
//---------------------------------------------------
template<typename T>
class AssetPool : public BaseAssetPool
{
public:
	using AssetType = T;

	virtual void LoadAsset(std::string a_filePath) override
	{
		m_Pool.push_back(std::tuple<std::string, T>(a_filePath,T(a_filePath)));
	}

	virtual void RetrieveAsset(BaseAssetRequestEvent* a_assetRequest) override
	{
		auto request = static_cast<AssetRequestEvent<AssetType>*>(a_assetRequest);
		request->m_AssetName = "Found Asset";
		request->Asset = AssetType("FoundAsset");

	}


	virtual void DisplayInEditor()
	{
		//Display all Assets of type T in the editor
		for (auto it = m_Pool.begin(); it < m_Pool.end(); ++it)
		{
			//display selectable with name given from first element of tuple
			//ImGui::Selectable(std::get<0>(it*), nullptr);
		}
	}

private:
	std::vector<std::tuple<std::string, T>> m_Pool;

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

	template<typename T>
	void AddResourceType(std::string typeName);

	void AddResource(std::string AssetType, std::string FilePath);
	void DisplayDirectoryContents();


	void HandleAssetRequestEvent(BaseAssetRequestEvent* request);

private:
	void DisplayDirectoryRecursive(const std::experimental::filesystem::path& a_pathToShow, int a_level);

	std::thread m_WorkerThread;
	std::map<std::string, BaseAssetPool*> m_PoolMap;
	bool IsWindowActive = false;
	std::experimental::filesystem::path m_AssetDirectory;
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
		m_PoolMap.insert(std::map<std::string, BaseAssetPool*>::value_type(typeName, pool));
	}
}


#endif
