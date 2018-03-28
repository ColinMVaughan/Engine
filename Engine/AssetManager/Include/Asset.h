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


//----------------------------------------------------------------
#define SERIALIZE_ASSET(ASSET)\
		void serialize_asset(EventManager& manager) \
		{ \
			ASSET.RequestAsset(manager);\
		}\

//------------------------------------------------------------------
#define EDITOR_REQUEST_ASSET(ASSET_TYPE,ASSET,NAME)									\
		ImGui::Text(NAME);															\
		ImGui::SameLine();															\
		ImGui::Selectable(ASSET.m_AssetName.c_str(), true);							\
		/*If something is being dragged/dropped into our window */					\
		if (ImGui::BeginDragDropTarget())											\
		{																			\
			/*If this is something we can accept*/									\
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(NAME))	\
			{																		\
				/*Assert that this data is the correct size*/						\
				IM_ASSERT(payload->DataSize == sizeof(BaseAssetRequestEvent*));		\
																					\
				BaseAssetRequestEvent* base = nullptr;								\
				memcpy(&base, payload->Data, sizeof(BaseAssetRequestEvent*));		\
																					\
				auto assetRequest = static_cast<AssetRequestEvent<ASSET_TYPE>*>(base);\
				ASSET.m_AssetName = assetRequest->m_AssetName;						\
				ASSET.m_AssetType = NAME;											\
				ASSET.m_Asset = *assetRequest->Asset;								\
																					\
			}																		\
		ImGui::EndDragDropTarget();													\
		}																			\



#endif
