#include "AssetManager.h"

namespace fs = std::experimental::filesystem;

AssetManager::AssetManager(const std::string &a_assetDirectory)
	:m_AssetDirectory(a_assetDirectory)
{
	
}

void AssetManager::AddResource(std::string AssetType, std::string FilePath)
{
	m_PoolMap[AssetType]->LoadAsset(FilePath);
}

void AssetManager::DisplayDirectoryContents()
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
				if(ImGui::TreeNode(filename.generic_string().c_str()))
				{
					DisplayDirectoryRecursive(entry, a_level + 1);
					ImGui::TreePop();
				}

			}

			else if (fs::is_regular_file(entry.status()))
				ImGui::Text(filename.generic_string().c_str());
		}
	}
}

void AssetManager::HandleAssetRequestEvent(BaseAssetRequestEvent * request)
{
	m_PoolMap[request->m_AssetTypeName]->RetrieveAsset(request);
	return;
}
