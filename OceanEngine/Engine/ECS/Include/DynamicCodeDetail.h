#pragma once
#include "ECS.h"
#include "Detail.h"

namespace ECS
{
	namespace DynamicDetail
	{



		typedef bool(*DynamicComponentFunc)(Scene*, Entity, ECS::detail::ComponentAction);
		typedef bool(*DynamicSystemFunc)(Scene*, ECS::detail::SystemAction);

		typedef std::map<std::string, DynamicComponentFunc> DynamicComponentRegistry;
		typedef std::map<std::string, DynamicSystemFunc> DynamicSystemRegistry;


		inline DynamicComponentRegistry& GetDynamicComponentRegistry()
		{
			static DynamicComponentRegistry reg;
			return reg;
		}

		inline DynamicSystemRegistry& GetDynamicSystemRegistry()
		{
			static DynamicSystemRegistry reg;
			return reg;
		}







		//Registry entry stores a string mapped with a component
		template<class T>
		struct DynamicRegistryEntry
		{
		public:
			static DynamicRegistryEntry<T>& Instance(const std::string& name)
			{
				static DynamicRegistryEntry<T> inst(name);
				return inst;
			}
		private:
			DynamicRegistryEntry(const std::string& name)
			{
				DynamicComponentRegistry& reg = GetDynamicComponentRegistry();
				DynamicComponentFunc func = detail::createComponent<T>;

				std::pair<DynamicComponentRegistry::iterator, bool> ret =
					reg.insert(DynamicComponentRegistry::value_type(name, func));

				if (ret.second == false)
				{
				}
			}

			DynamicRegistryEntry(const DynamicRegistryEntry<T>&) = delete;
			DynamicRegistryEntry& operator=(const DynamicRegistryEntry<T>&) = delete;
		};



		//Registry entry stores a string mapped with a component
		template<class T>
		struct DynamicSystemRegistryEntry
		{
		public:
			static DynamicSystemRegistryEntry<T>& Instance(const std::string& name)
			{
				static DynamicSystemRegistryEntry<T> inst(name);
				return inst;
			}
		private:
			DynamicSystemRegistryEntry(const std::string& name)
			{
				DynamicSystemRegistry& reg = GetDynamicSystemRegistry();
				DynamicSystemFunc func = detail::createSystem<T>;

				std::pair<DynamicSystemRegistry::iterator, bool> ret =
					reg.insert(DynamicSystemRegistry::value_type(name, func));

				if (ret.second == false)
				{
				}
			}

			DynamicSystemRegistryEntry(const DynamicSystemRegistryEntry<T>&) = delete;
			DynamicSystemRegistryEntry& operator=(const DynamicSystemRegistryEntry<T>&) = delete;
		};
	}

}