#ifndef ECS_DETAIL_H
#define ECS_DETAIL_H

#include <map>
#include <string>
#include <utility>
#include "ECS.h"

class Component;

namespace ECS
{
	namespace detail
	{
		typedef void (*CreateComponentFunc)(Scene*,Entity);
		typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;

		inline ComponentRegistry& GetComponentRegistry()
		{
			static ComponentRegistry reg;
			return reg;
		}

		template<class T>
		void createComponent(Scene* scene, Entity entity)
		{
			scene->AddComponent<T>(entity);
		}

		template<class T>
		struct RegistryEntry
		{
		public:
			static RegistryEntry<T>& Instance(const std::string& name)
			{
				static RegistryEntry<T> inst(name);
				return inst;
			}
		private:
			RegistryEntry(const std::string& name)
			{
				ComponentRegistry& reg = GetComponentRegistry();
				CreateComponentFunc func = createComponent<T>;

				std::pair<ComponentRegistry::iterator, bool> ret =
					reg.insert(ComponentRegistry::value_type(name, func));

				if (ret.second == false)
				{
				}
			}

			RegistryEntry(const RegistryEntry<T>&) = delete;
			RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
		};
	}
}

#endif