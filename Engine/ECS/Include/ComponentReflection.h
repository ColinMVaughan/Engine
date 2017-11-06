#ifndef COMPONENT_RELECTION_H
#define COMPONENT_RELECTION_H

#include <string>
#include "Detail.h"

class Component
{

};

namespace ECS
{
	void AddComponentFromString(const std::string& name, Scene* scene, Entity entity);
	unsigned int GetRegisteredComponentNumber();
	std::string GetComponentName(unsigned int index);


	void destroy(const Component* comp);
}

#define COMPONENT_REGISTER(TYPE, NAME)									\
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

#endif