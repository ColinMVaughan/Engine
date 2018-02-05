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


#include <imgui.h>
#include <EventManager.h>

#include <thread>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <functional>



//--------------------------------------------------
// BaseAssetPool
//--------------------------------------------------

class BaseAssetPool
{
public:
	virtual void LoadAsset(std::string filePath) = 0;
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


	virtual void LoadAsset(std::string filePath) override
	{
		//m_Pool.push_back(T(filePath));
	}


	virtual void DisplayInEditor()
	{
		//Display all Assets of type T in the editor
		for (auto it = m_Pool.begin(); it < m_Pool.end(); ++it)
		{
			//display selectable with name given from first element of tuple
			ImGui::Selectable(std::get<0>(it*), nullptr);
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

	void GetResourceType()
	{
		
	}
	//Display the AssetManager window. Allowing the user to view and add different 
	
	void DisplayInEditor()
	{
		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	}

private:
	std::thread m_WorkerThread;
	std::map<std::string, BaseAssetPool*> m_PoolMap;
	bool IsWindowActive = false;
};


#define LOAD_ASSET_TYPE()    \
		void LoadAssetType() \
		{					 \
							 \
		}					 \


#endif