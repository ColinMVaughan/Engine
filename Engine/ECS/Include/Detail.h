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

//Here is where we define possible compile time function checks
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

		typedef bool (*CreateComponentFunc)(Scene*,Entity, ComponentAction);
		typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;

		inline ComponentRegistry& GetComponentRegistry()
		{
			static ComponentRegistry reg;
			return reg;
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
				if constexpr(is_detected_v<has_detail::has_serialization, T>)
				{
					scene->SerializeComponent<T>(entity);
				}
				break;

			case Load:
				if constexpr(is_detected_v<has_detail::has_serialization, T>)
				{
					scene->UnserializeComponent<T>(entity);
				}
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
	}
}

#endif