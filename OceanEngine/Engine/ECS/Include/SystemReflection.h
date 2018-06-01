#pragma once
#include "Detail.h"
#include <DynamicCodeDetail.h>
#include<string>

class System {};

namespace ECS
{

	void AddSystemFromString(const std::string& name, Scene* scene);

}

#ifdef _USRDLL

#define SYSTEM_REGISTER(TYPE, NAME) 

#else

//Registers the component type with a string in the global component register.
//Registered components can be added to entities from the editor, have their
//data exposed to the editor, and be serialized.

//PARAMETERS:
//TYPE = the class type of the component
//NAME = a string corrisponding with the name of the component
#define SYSTEM_REGISTER(TYPE, NAME)									    \
	namespace ECS{														\
	namespace detail {													\
	namespace															\
	{																	\
																		\
		template<class T>												\
		class SystemRegistration;										\
																		\
		template<>														\
		class SystemRegistration<TYPE>									\
		{																\
			static const ::ECS::detail::SystemRegistryEntry<TYPE>& reg;	\
		};																\
																		\
		const ::ECS::detail::SystemRegistryEntry<TYPE>&					\
			SystemRegistration<TYPE>::reg =								\
			::ECS::detail::SystemRegistryEntry<TYPE>::Instance(NAME);	\
																		\
	}																	\
} }								


#endif


//Registers the component type with a string in the global component register.
//Registered components can be added to entities from the editor, have their
//data exposed to the editor, and be serialized.

//PARAMETERS:
//TYPE = the class type of the component
//NAME = a string corrisponding with the name of the component
#define USER_SYSTEM_REGISTER(TYPE, NAME)									\
	namespace ECS{														\
	namespace DynamicDetail {													\
	namespace															\
	{																	\
																		\
		template<class T>												\
		class DynamicSystemRegistration;									\
																		\
		template<>														\
		class DynamicSystemRegistration<TYPE>								\
		{																\
			static const ::ECS::DynamicDetail::DynamicSystemRegistryEntry<TYPE>& reg;		\
		};																\
																		\
		const ::ECS::DynamicDetail::DynamicSystemRegistryEntry<TYPE>&						\
			DynamicSystemRegistration<TYPE>::reg =							\
			::ECS::DynamicDetail::DynamicSystemRegistryEntry<TYPE>::Instance(NAME);			\
																		\
	}																	\
} }		