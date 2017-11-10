#ifndef ECS_DETAIL_H
#define ECS_DETAIL_H

#include <map>
#include <string>
#include <utility>
#include <type_traits>
#include "ECS.h"

class Component;


namespace has_detail
{
	template<template<typename> typename Op, typename T, typename = void>
	struct is_detected : std::false_type {};

	template<template<typename> typename Op, typename T>
	struct is_detected<Op, T, std::void_t<Op<T>>> : std::true_type {};
}

template<template<typename> typename Op, typename T>
static constexpr bool is_detected_v = has_detail::is_detected<Op, T>::value;

namespace has_detail
{
	template<class U>
	using has_init = decltype(std::declval<U>().Initalize());

	template <class U>
	using has_expose = decltype(std::declval<U>().ExposeToEditor());
}





namespace ECS
{
	namespace detail
	{
		enum ComponentAction
		{
			Add = 0, Check, Remove, Display
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
			}

			return true;
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