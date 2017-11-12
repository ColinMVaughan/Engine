#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <thread>
#include <vector>
#include <string>
#include <map>

class BaseAssetPool
{
public:

	virtual void LoadAsset(std::string filePath) = 0;
	//virtual void UnloadPool() = 0;
};

//---------------------------------------------------
//					Asset Pool
//
// Holds a vector of asset type T.
//
// 
//---------------------------------------------------

template<typename T>
class AssetPool : public BaseAssetPool
{
public:


	virtual void LoadAsset(std::string filePath) override
	{
		//m_Pool.push_back(T(filePath));
	}

private:
	std::vector<T> m_Pool;
};


//------------------------------------------------------------------
class AssetManager
{
public:

	template<typename T>
	void AddResourceType(std::string typeName)
	{
		//Check that the pool dosen't already exist
		auto iterator = m_PoolMap.find(typeName);
		if (iterator == m_PoolMap.end())
		{
			BaseAssetPool* pool = new AssetPool<T>();
			m_PoolMap.insert(std::map<std::string, BaseAssetPool*>::value_type(typeName, pool));
		}
	}

private:
	std::thread m_WorkerThread;
	std::map<std::string, BaseAssetPool*> m_PoolMap;
};

#endif