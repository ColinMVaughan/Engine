#include "AssetManager.h"

namespace fs = std::experimental::filesystem;

AssetManager::AssetManager(const std::string &a_assetDirectory)
	:m_AssetDirectory(a_assetDirectory), m_SelectedPath(a_assetDirectory)
{
	
}

void AssetManager::AddResource(std::string AssetType, std::string FilePath)
{
	m_PoolMap[AssetType]->LoadAsset(FilePath);
}

void AssetManager::DisplayAssetDirectory()
{
	DisplayDirectoryRecursive(m_AssetDirectory, 0);
}


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

void AssetManager::DisplayDirectoryContents()
{
	if(fs::exists(m_SelectedPath) && fs::is_directory(m_SelectedPath))
	{
		for(const auto& asset : fs::directory_iterator(m_SelectedPath))
		{
			
			auto filename = asset.path().filename();
			if(!fs::is_directory(asset.status()))
			{
				ImGui::Text(filename.generic_string().c_str());
			}
		}
	}
}


void AssetManager::HandleAssetRequestEvent(BaseAssetRequestEvent * a_request)
{
	m_PoolMap[a_request->GetAssetTypeName()]->RetrieveAsset(a_request);
	return;
}
