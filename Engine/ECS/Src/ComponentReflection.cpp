#include "ComponentReflection.h"
#include <string>
#include "Detail.h"

void ECS::AddComponentFromString(const std::string& name, Scene* scene, Entity entity)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.find(name);

	if (it == reg.end())
		return;

	detail::CreateComponentFunc func = it->second;
	return func(scene, entity);
}

unsigned int ECS::GetRegisteredComponentNumber()
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	return reg.size();
}

std::string ECS::GetComponentName(unsigned int index)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.begin();
	return std::string();
}

void ECS::destroy(const Component* comp)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();

	for (auto it = reg.begin(); it != reg.end(); ++it)
	{
		std::cout << it->first;
	}

}