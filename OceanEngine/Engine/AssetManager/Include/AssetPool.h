#ifndef _ASSET_POOL_H_
#define _ASSET_POOL_H_


#include <string>
#include <functional>
#include "AssetEvent.h"

//--------------------------------------------------
// BaseAssetPool
//--------------------------------------------------

class BaseAssetPool
{
public:
	//virtual void LoadAsset(std::experimental::filesystem::path assetPath) = 0;
	virtual bool RetrieveAsset(BaseAssetRequestEvent* request) = 0;
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

	virtual void AddAsset(std::experimental::filesystem::path assetPath, T Asset)
	{
		m_Pool.insert(std::make_pair(assetPath.string(), Asset));
	}

	virtual bool RetrieveAsset(BaseAssetRequestEvent* a_assetRequest) override
	{
		auto request = static_cast<AssetRequestEvent<AssetType>*>(a_assetRequest);

		std::map<std::string, T>::iterator it;
		it = m_Pool.find(request->m_AssetName);

		if (it == m_Pool.end())
			return false;

		request->Asset = &it->second;
		return true;
	}


private:
	//std::vector<std::tuple<std::string, T>> m_Pool;
	std::map<std::string, T> m_Pool;

};

#endif