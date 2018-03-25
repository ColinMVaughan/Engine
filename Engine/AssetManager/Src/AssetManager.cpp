#include "AssetManager.h"
#include "AssetRegistration.h"
#include <iostream>

namespace fs = std::experimental::filesystem;

AssetManager::AssetManager(const std::string &a_assetDirectory)
	:m_AssetDirectory(a_assetDirectory), m_SelectedPath(a_assetDirectory)
{
}

AssetManager::AssetManager(const std::string &a_assetDirectory, EventManager & eManager)
	: m_AssetDirectory(a_assetDirectory), m_SelectedPath(a_assetDirectory)
{
	//REGISTER_EVENT_LISTNER(BaseAssetRequestEvent, AssetManager::HandleAssetRequestEvent, eManager)
	//eManager.AddListner<BaseAssetRequestEvent>(std::bind(&AssetManager::HandleAssetRequestEvent, this, std::placeholders::_1));
	std:std::function<void(BaseAssetRequestEvent&)> f2 = [this](BaseAssetRequestEvent& eve) {HandleAssetRequestEvent(eve); };
	eManager.AddListner<BaseAssetRequestEvent>(f2);
}

//-----------------------------------
// Add an asset to an existing pool
//
//------------------------------------
void AssetManager::AddResource(std::string AssetType, std::experimental::filesystem::path assetPath)
{
	std::map<std::string, BaseAssetPool*>::iterator it = m_PoolMap.find(AssetType);

	if (it == m_PoolMap.end())
		return;

	it->second->LoadAsset(assetPath);
}

void AssetManager::DisplayAssetDirectory()
{
	DisplayDirectoryRecursive(m_AssetDirectory, 0);
}

//Display the folder structure of the directory
void AssetManager::DisplayDirectoryRecursive(const std::experimental::filesystem::path& a_pathToShow, int a_level)
{

	if (fs::exists(a_pathToShow) && fs::is_directory(a_pathToShow))
	{
		for (const auto& entry : fs::directory_iterator(a_pathToShow))
		{
			auto filename = entry.path().filename();
			if (fs::is_directory(entry.status()))
			{
				ImGuiTreeNodeFlags directoryFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((entry.path() == m_SelectedPath)? ImGuiTreeNodeFlags_Selected: 0);
				bool isNodeOpen = ImGui::TreeNodeEx(filename.generic_string().c_str(), directoryFlags);
				
				if (ImGui::IsItemClicked())
					m_SelectedPath = entry.path();

				if(isNodeOpen)
				{
					DisplayDirectoryRecursive(entry, a_level + 1);
					ImGui::TreePop();
				}
			}
		}
	}
}

//Display the contents of the currently selected directory
void AssetManager::DisplayDirectoryContents()
{
	if(fs::exists(m_SelectedPath) && fs::is_directory(m_SelectedPath))
	{
		for(const auto& asset : fs::directory_iterator(m_SelectedPath))
		{
			//Get Filename & check if it is a directory
			auto filename = asset.path().filename();
			if(!fs::is_directory(asset.status()))
			{
				//Print the name of the file
				ImGui::Selectable(filename.generic_string().c_str());
				if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::Text(filename.string().c_str());

					BaseAssetRequestEvent* baseAsset = Assets::RequestAsset(this, asset.path());

					if(baseAsset)
						ImGui::SetDragDropPayload(baseAsset->GetAssetTypeName().c_str(), &baseAsset, sizeof(BaseAssetRequestEvent*), ImGuiCond_Once);
				

					ImGui::EndDragDropSource();
				}
			}
		}
	}
}


bool AssetManager::HandleAssetRequestEvent(BaseAssetRequestEvent& a_request)
{
	std::map<std::string, BaseAssetPool*>::iterator it = m_PoolMap.find(a_request.GetAssetTypeName());

	if (it == m_PoolMap.end())
		return false;

	return it->second->RetrieveAsset(&a_request);
}

//Load all compatible files in the asset directory.
void AssetManager::LoadAllAssets()
{
	LoadAssetsRecursive(m_AssetDirectory);
}

//Iterate through asset directory and find files. 
//once a file is found, check if the file extension is registered, and if so, attempt to load it.
void AssetManager::LoadAssetsRecursive(const std::experimental::filesystem::path & a_currentPath)
{
	//----------------------------------------------------------------------------------------------------
	if (fs::exists(a_currentPath) && fs::is_directory(a_currentPath))
	{
		for (const auto& entry : fs::directory_iterator(a_currentPath))
		{
			//If the current path isn't a file: recurse down
			auto filename = entry.path().filename();
			if (fs::is_directory(entry.status()))
			{
				LoadAssetsRecursive(entry.path());
			}
			else
			{
				//if the current path is a file: attempt to load it
				Assets::LoadNewAsset(this, entry.path());
			}

		}
	}
	return;
}

