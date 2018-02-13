#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

//----------------------------------------------------------------------------
//					Notes to Self
// *The Asset Manager needs to be reworked
//		-How do we differentiate project assets from scene assets?
//		-How should users add assets to the scene?
//		-The asset manager should probably be seperate from all UI, right?
//
//	*Asset Access/Storage/Retreaval should probably be hooked into Events...
//		-We should get Events up and running before we worry about assets.
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------
//					Notes to Self 2
//
// *We may want to make asset manager its own project. 
//		-Aset Management is probabbly a core feature.
//		-Im thinking 
//--------------------------------------------------------------------------
#include <imgui.h>
#include <EventManager.h>

#include <thread>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <functional>



class BaseARE : public IEvent
{
public:
	std::string m_AssetTypeName;
};

class AssetRequestEvent : public BaseARE
{
public:
	std::string m_AssetName;
};


//--------------------------------------------------
// BaseAssetPool
//--------------------------------------------------

class BaseAssetPool
{
public:
	virtual void LoadAsset(std::string filePath) = 0;
	virtual void RetrieveAsset(BaseARE* request) = 0;
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

	virtual void LoadAsset(std::string filePath) override
	{
		m_Pool.push_back(std::tuple<std::string, T>);
	}

	virtual void RetrieveAsset(BaseARE* assetRequest) override
	{
		auto request = static_cast<AssetRequestEvent*>(assetRequest);
		request->m_AssetName = "Found Asset";

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

	void AddResource(std::string AssetType, std::string FilePath)
	{
		m_PoolMap[AssetType]->LoadAsset(FilePath);
	}

	void GetResourceType()
	{
		
	}

	//Display the AssetManager window. Allowing the user to view and add different Assets.
	//Assets are grouped into the same type.
	void DisplayInEditor()
	{
		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	}


	void HandleAssetRequestEvent(BaseARE* request)
	{
		m_PoolMap[request->m_AssetTypeName]->RetrieveAsset(request);
		return;
	}


private:
	std::thread m_WorkerThread;
	std::map<std::string, BaseAssetPool*> m_PoolMap;
	bool IsWindowActive = false;
};


#endif