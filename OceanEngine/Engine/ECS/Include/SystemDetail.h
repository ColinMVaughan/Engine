#pragma once
#include "ECS.h"

class BaseSystem;

namespace ECS
{
	namespace detail
	{

		enum SystemAction
		{
			Add = 0
		};


		typedef bool (CreateSystemFunc)(Scene*, SystemAction);
		typedef std::map<std::string, CreateSystemFunc> SystemRegistry;


		inline SystemRegistry& GetSystemRegistry()
		{
			static SystemRegistry reg;
			return reg;
		}


		template<class T>
		bool createSystem(Scene* scene, Entity entity, SystemAction action)
		{

			switch (action)
			{
			case Add:
				scene->AddSystem<T>();
				break;
			}
			return true;
		}



		//Registry entry stores a string mapped with a component
		template<class T>
		struct SystemRegistryEntry
		{
		public:
			static SystemRegistryEntry<T>& Instance(const std::string& name)
			{
				static SystemRegistryEntry<T> inst(name);
				return inst;
			}
		private:
			SystemRegistryEntry(const std::string& name)
			{
				SystemRegistry& reg = SystemRegistry();
				CreateSystemFunc func = createSystem<T>;

				std::pair<SystemRegistry::iterator, bool> ret =
					reg.insert(SystemRegistry::value_type(name, func));

				if (ret.second == false)
				{
				}
			}

			SystemRegistryEntry(const SystemRegistryEntry<T>&) = delete;
			SystemRegistryEntry& operator=(const SystemRegistryEntry<T>&) = delete;
		};



	}
}