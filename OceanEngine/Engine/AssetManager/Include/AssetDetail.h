#ifndef _ASSET_DETAIL_H_
#define _ASSET_DETAIL_H_

#include <string>
#include <type_traits>
#include <utility>
#include <filesystem>

#include "AssetManager.h"
#include "AssetLoader.h"

namespace Assets
{
	namespace detail
	{
		enum AssetActions
		{
			Load = 0, Retrieve, Remove, CheckType
		};

		typedef bool (*AssetFunction)(AssetManager*, BaseAssetRequestEvent*, std::experimental::filesystem::path, std::string, AssetActions);
		typedef std::map < std::string, std::tuple<std::string, AssetFunction>> AssetRegistry;

		inline AssetRegistry& GetAssetRegistry()
		{
			static AssetRegistry reg;
			return reg;
		}


		template<class T>
		bool RequestAsset(AssetManager* manager, BaseAssetRequestEvent* request, std::experimental::filesystem::path asset, std::string typeName, AssetActions action)
		{
			switch (action)
			{
			case Load:

				manager->AddResourceType<T>(typeName);
				manager->AddResource(typeName, asset);
				return true;
				break;

			case Retrieve:

				AssetRequestEvent<T>* RE = new AssetRequestEvent<T>();
				RE->m_AssetName = asset.string();
				RE->m_AssetTypeName = typeName;

				request = RE;

				if(manager->HandleAssetRequestEvent(request))
				return true;

				break;

			case CheckType:
				break;
			}

			return false;
		}

		template<class T>
		struct AssetRegistryEntry
		{
		public:
			static AssetRegistryEntry<T>& Instance(const std::string& extension, const std::string& name)
			{
				static AssetRegistryEntry<T> inst(extension, name);
				return inst;
			}

		private:
			AssetRegistryEntry(const std::string& extension, const std::string& name)
			{
				AssetRegistry& reg = GetAssetRegistry();
				AssetFunction func = RequestAsset<T>;

				std::pair<AssetRegistry::iterator, bool> ret =
					reg.insert(AssetRegistry::value_type(extension, 
						std::tuple<std::string, AssetFunction>(name, func)));

				if (ret.second == false)
				{
				}
			}

			AssetRegistryEntry(const AssetRegistryEntry<T>&) = delete;
			AssetRegistryEntry& operator=(const AssetRegistryEntry<T>&) = delete;
		};




		//------------------------------------------------------------------
		//
		//-------------------------------------------------------------------
		


		typedef std::function<void(AssetManager&, std::experimental::filesystem::path)> FileLoadFunction;
		typedef std::map < std::string, BaseAssetLoader*> FileRegistry;
		inline FileRegistry& GetFileRegistry()
		{
			static FileRegistry reg;
			return reg;
		}


		template<typename T>
		struct FileRegistryEntry
		{
		public:
			static FileRegistryEntry<T>& Instance(const std::string& extension)
			{
				static_assert(std::is_base_of_v<BaseAssetLoader, T>);
				static FileRegistryEntry<T> inst(extension, new T);
				return inst;

			}

		private:
			FileRegistryEntry(const std::string& extension, T* loader)
			{
				FileRegistry& reg = GetFileRegistry();

				std::pair<FileRegistry::iterator, bool> ret =
					reg.insert(FileRegistry::value_type(extension, loader));

				if (ret.second == false)
				{
				}
			}

			FileRegistryEntry(const FileRegistryEntry<T>&) = delete;
			FileRegistryEntry& operator=(const FileRegistryEntry<T>&) = delete;
		};

	}
}

#endif