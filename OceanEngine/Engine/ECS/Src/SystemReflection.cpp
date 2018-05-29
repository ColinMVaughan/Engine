#include "SystemReflection.h"

void ECS::AddSystemFromString(const std::string & name, Scene * scene)
{
	//Get a reference to the global component registry
	detail::SystemRegistry& reg = detail::GetSystemRegistry();
	//look to see if the component is registered
	detail::SystemRegistry::iterator it = reg.find(name);

	//if the component is not registered, return false
	if (it == reg.end())
		return;

	//Get the function pointer associated with the component
	detail::CreateSystemFunc func = it->second;

	//call the function with the "add" component action
	func(scene, detail::SystemAction::AddSystem);
	return;
}
