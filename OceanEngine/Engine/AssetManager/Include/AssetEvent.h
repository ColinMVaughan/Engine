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
	std::string m_AssetName;
};

template<typename T>
class AssetRequestEvent : public BaseAssetRequestEvent
{
public:
	T* Asset;
private:

};


template<typename T>
class AssetLoadEvent : public BaseAssetRequestEvent
{
public:
	T* Asset;
private:

};

#endif