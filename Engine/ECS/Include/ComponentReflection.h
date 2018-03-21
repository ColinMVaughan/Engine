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
	bool CheckComponentFromString(const std::string& name, Scene* scene, Entity entity);
	bool DisplayComponentParameters(const std::string& name, Scene* scene, Entity entity);


	bool SerializeComponent(const std::string& name, Scene* scene, Entity entity);
	bool UnSerializeComponent(const std::string& name, Scene* scene, Entity entity);

	unsigned int GetRegisteredComponentNumber();
	std::string GetComponentName(unsigned int index);


	void destroy(const Component* comp);
}

//Registers the component type with a string in the global component register.
//Registered components can be added to entities from the editor, have their
//data exposed to the editor, and be serialized.

//PARAMETERS:
//TYPE = the class type of the component
//NAME = a string corrisponding with the name of the component
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


// Allows the engine to serialize the data of the component during saving / loading.
// The Component Must also Register with the engine through the COMPONENT_REGISTER macro.

//Meant for when you want default serialization behaviour
//PARAMETERS:
// "..." = Names of variables you want to serialize
#define COMPONENT_SERIALIZE(...)					\
	template<typename Archive>						\
	void serialize(Archive & arc)					\
	{												\
		arc(__VA_ARGS__);							\
	}


//PARAMETERS:
// "..." = Names of variables you want to serialize
#define COMPONENT_SAVE(...)			\
	template<typename Archive>		\
	void save(Archive & arc) const	\
	{								\
		arc(__VA_ARGS__);			\
	}							

//PARAMETERS:
// "..." = Names of variables you want to serialize
#define COMPONENT_LOAD(...)		\
	template<typename Archive>	\
	void save(Archive & arc)	\
	{							\
		arc(__VA_ARGS__);		\
	}	

#define ASSET_SERIALIZE(ASSET_NAME)					\
	void serialize_asset(EventManager& manager)		\
	{												\
		ASSET_NAME.RequestAsset(manager);			\
	}												\

#endif