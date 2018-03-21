#ifndef _ASSET_H_
#define _ASSET_H_
#include <EventManager.h>
#include <AssetEvent.h>

template<typename T>
class Asset
{
public:
	Asset() = default;
	~Asset() = default;

	void RequestAsset(EventManager& manager)
	{
		AssetRequestEvent<T> request;
		request.m_AssetName = m_AssetName;
		request.m_AssetTypeName = m_AssetType;

		BaseAssetRequestEvent* baseRequest = &request;

		manager.DispatchEvent<BaseAssetRequestEvent>(*baseRequest);

		m_Asset = *request.Asset;
		//m_AssetName = request.m_AssetName;
	}

	template<typename Archive>
	void serialize(Archive& arc)
	{
		arc(m_AssetName, m_AssetType);
	}

	T m_Asset;
	std::string m_AssetName;
	std::string m_AssetType;
};


#endif
