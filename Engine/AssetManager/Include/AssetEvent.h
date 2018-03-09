#ifndef _ASSET_EVENT_H_
#define _ASSET_EVENT_H_

#include <EventManager.h>

//-----------------------------------------------
//			AssetRequestEvent
//
// These events are used to request assets for components
//----------------------------------------------

class BaseAssetRequestEvent : public IEvent
{
public:
	std::string GetAssetTypeName()
	{
		return m_AssetTypeName;
	}
	std::string m_AssetTypeName;
};

template<typename T>
class AssetRequestEvent : public BaseAssetRequestEvent
{
public:

	std::string m_AssetName;
	T* Asset;
private:

};

#endif