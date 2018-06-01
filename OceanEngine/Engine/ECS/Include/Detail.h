#ifndef ECS_DETAIL_H
#define ECS_DETAIL_H

//--------------------------------------------------------------
//					DETAIL.h
//					Colin Vaughan
//
// The detail file includes template functionality for registering
// and modifying components.
//
// This allows components to be added, removed, checked, and
// modified arbitrarily inside the editor, while still working 
// with my heavily templated ECS library.
//----------------------------------------------------------------

#include <map>
#include <string>
#include <utility>
#include <type_traits>
#include "ECS.h"


class Component;

//Template detection boilerplate
namespace has_detail
{
	template<template<typename> typename Op, typename T, typename = void>
	struct is_detected : std::false_type {};

	template<template<typename> typename Op, typename T>
	struct is_detected<Op, T, std::void_t<Op<T>>> : std::true_type {};
}

template<template<typename> typename Op, typename T>
static constexpr bool is_detected_v = has_detail::is_detected<Op, T>::value;

//	Here is where we define possible compile time function checks.
//	If a registered component defines any of these functions it will be called,
//	otherwise it will not. Without the added complexity and performance of virtual functions and inheritance.
namespace has_detail
{
	//Registered component has an Initalize() function?
	template<class U>
	using has_init = decltype(std::declval<U>().Initalize());

	//Registered component has an ExposeToEditor() function?
	template <class U>
	using has_expose = decltype(std::declval<U>().ExposeToEditor());

	//Registered component has a Serialize() function?
	template <class U>
	using has_serialization = decltype(std::declval<U>().serialize(std::declval<U&>()));

	template <class U>
	using has_serialization_save = decltype(std::declval<U>().save(std::declval<U&>()));

	template <class U>
	using has_serialization_load = decltype(std::declval<U>().load(std::declval<U&>()));

	template<class U>
	using has_serialization_asset = decltype(std::declval<U>().serialize_asset(std::declval<EventManager&>()));
}



namespace ECS
{
	namespace detail
	{
		//Posible component actions from the editor
		enum ComponentAction
		{
			Add = 0, Check, Remove, Display, Save, Load
		};

		enum SystemAction
		{
			AddSystem = 0
		};

		typedef bool (*CreateComponentFunc)(Scene*,Entity, ComponentAction);
		typedef bool(*CreateSystemFunc)(Scene*, SystemAction);

		typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;
		typedef std::map<std::string, CreateSystemFunc> SystemRegistry;

		inline ComponentRegistry& GetComponentRegistry()
		{
			static ComponentRegistry reg;
			return reg;
		}

		inline SystemRegistry& GetSystemRegistry()
		{
			static SystemRegistry sysreg;
			return sysreg;
		}


		template<class T>
		bool createComponent(Scene* scene, Entity entity, ComponentAction action)
		{
			switch (action)
			{
			case Add:
				scene->AddComponent<T>(entity);
				break;

			case Check:
				return scene->HasComponent<T>(entity);
				break;

			case Display:
				if constexpr(is_detected_v<has_detail::has_expose, T>)
				{
					scene->GetComponent<T>(entity)->ExposeToEditor();
				}
				break;

			case Save:
				if constexpr(is_detected_v<has_detail::has_serialization, T> || is_detected_v<has_detail::has_serialization_save, T>)
				{
					scene->SerializeComponent<T>(entity);
				}
				break;

			case Load:
				if constexpr(is_detected_v<has_detail::has_serialization, T> || is_detected_v<has_detail::has_serialization_load, T>)
				{
					scene->UnserializeComponent<T>(entity);
					if constexpr(is_detected_v<has_detail::has_serialization_asset, T>)
					{
						scene->GetComponent<T>(entity)->serialize_asset(scene->GetEventManager());
					}
				}
				break;
			}

			return true;
		}

		template<typename T>
		bool createSystem(Scene* scene, SystemAction action)
		{
			switch (action)
			{
			case AddSystem:
				scene->AddSystem<T>();
				break;
			}

			return true;
		}


		//Registry entry stores a string mapped with a component
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
				SystemRegistry& reg = GetSystemRegistry();
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

#endif