#pragma once


#include <DynamicCodeDetail.h>

#define USER_COMPONENT_REGISTER(TYPE, NAME)								\
	namespace ECS{														\
	namespace detail {													\
	namespace															\
	{																	\
																		\
		template<class T>												\
		class ComponentRegistration;									\
																		\
		template<>														\
		class ComponentRegistration<TYPE>								\
		{																\
			static const ::ECS::detail::RegistryEntry<TYPE>& reg;		\
		};																\
																		\
		const ::ECS::detail::RegistryEntry<TYPE>&						\
			ComponentRegistration<TYPE>::reg =							\
			::ECS::detail::RegistryEntry<TYPE>::Instance(NAME);			\
																		\
	}																	\
} }	