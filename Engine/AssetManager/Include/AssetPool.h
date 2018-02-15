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
		m_Pool.push_back(std::tuple<std::string, T>(a_filePath, T(a_filePath)));
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

#endif