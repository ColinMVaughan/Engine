#ifndef _ASSET_DETAIL_H_
#define _ASSET_DETAIL_H_

#include <string>
#include <type_traits>
#include <utility>
#include <filesystem>

#include "AssetManager.h"


namespace Assets
{
	namespace detail
	{
		enum AssetActions
		{
			Load = 0, Retrieve, Remove
		};

		typedef BaseAssetRequestEvent* (*AssetFunction)(AssetManager*, std::experimental::filesystem::path, std::string, AssetActions);
		typedef std::map < std::string, std::tuple<std::string, AssetFunction>> AssetRegistry;

		inline AssetRegistry& GetAssetRegistry()
		{
			static AssetRegistry reg;
			return reg;
		}


		template<class T>
		BaseAssetRequestEvent* RequestAsset(AssetManager* manager, std::experimental::filesystem::path asset, std::string typeName, AssetActions action)
		{
			switch (action)
			{
			case Load:

				manager->AddResourceType<T>(typeName);
				manager->AddResource(typeName, asset);
				return nullptr;
				break;

			case Retrieve:

				AssetRequestEvent<T>* RE = new AssetRequestEvent<T>();
				RE->m_AssetName = asset.string();
				RE->m_AssetTypeName = typeName;

				BaseAssetRequestEvent* BE = RE;

				if(manager->HandleAssetRequestEvent(*BE))
				return BE;

				break;
			}

			return nullptr;
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

	}
}

#endif