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
			Add = 0, Retrieve, Remove, CheckType
		};

		typedef bool (*AssetFunction)(AssetManager*, BaseAssetRequestEvent*, std::experimental::filesystem::path, std::string, AssetActions);
		typedef std::map < std::string, AssetFunction> AssetRegistry;

		inline AssetRegistry& GetAssetRegistry()
		{
			static AssetRegistry reg;
			return reg;
		}


		template<class T>
		bool AssetActionFunc(AssetManager* manager, BaseAssetRequestEvent* request, std::experimental::filesystem::path asset, std::string typeName, AssetActions action)
		{
			switch (action)
			{
			case Add:

				manager->AddAsset<T>()
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
			static AssetRegistryEntry<T>& Instance(const std::string& name)
			{
				static AssetRegistryEntry<T> inst(name);
				return inst;
			}

		private:
			AssetRegistryEntry(const std::string& name)
			{
				AssetRegistry& reg = GetAssetRegistry();
				AssetFunction func = AssetActionFunc<T>;

				std::pair<AssetRegistry::iterator, bool> ret =
					reg.insert(AssetRegistry::value_type(name, func));

				if (ret.second == false)
				{
				}
			}

			AssetRegistryEntry(const AssetRegistryEntry<T>&) = delete;
			AssetRegistryEntry& operator=(const AssetRegistryEntry<T>&) = delete;
		};













		//------------------------------------------------------------------
		// File Loading Function
		//
		// Registers a Asset loader with a file extension. eg. registers ".fbx" with FBXLoader class that will be used to load those
		// kinds of files.
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





















		//========================================================================================================
		class FileLoaderRegistry
		{
		public:

			static FileLoaderRegistry& instance(const std::string& id)
			{
				static std::map<std::string, FileLoaderRegistry> instances;
				auto it = instances.find(id);

				if (it != instances.end())
					return it->second;

				FileLoaderRegistry instance(id);
				auto insertIt = instances.emplace(id, std::move(instance));
				return insertIt.first->second;
			}

			~FileLoaderRegistry() = default;
			FileLoaderRegistry(FileLoaderRegistry&) = delete;
			FileLoaderRegistry(FileLoaderRegistry&&) = delete;
			FileLoaderRegistry& operator = (const FileLoaderRegistry&) = delete;
			FileLoaderRegistry& operator = (const FileLoaderRegistry&&) = delete;


			void TestFunc()
			{

			}

		private:
			FileLoaderRegistry(const std::string& id);
		};










	}
}


#endif