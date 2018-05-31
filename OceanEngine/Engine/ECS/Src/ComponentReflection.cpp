#include "ComponentReflection.h"
#include <string>
#include "Detail.h"
#include "DynamicCodeDetail.h"

//---------------------------------------------------------
//
//----------------------------------------------------------
void ECS::AddComponentFromString(const std::string& name, Scene* scene, Entity entity)
{
	//Get a reference to the global component registry
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	//look to see if the component is registered
	detail::ComponentRegistry::iterator it = reg.find(name);

	//if the component is not registered, return false
	if (it != reg.end())
	{
		//Get the function pointer associated with the component
		detail::CreateComponentFunc func = it->second;

		//call the function with the "add" component action
		func(scene, entity, detail::ComponentAction::Add);
		return;
	}

	//If the component can not be found in the core engine list, check user components
	DynamicDetail::DynamicComponentRegistry& d_reg = DynamicDetail::GetDynamicComponentRegistry();
	DynamicDetail::DynamicComponentRegistry::iterator d_it = d_reg.find(name);
	if (d_it != d_reg.end())
	{
		//Get the function pointer associated with the component
		DynamicDetail::DynamicComponentFunc func = d_it->second;

		//call the function with the "add" component action
		func(scene, entity, detail::ComponentAction::Add);
		return;
	}
	
	return;
}

//-----------------------------------------------------------
//
//-----------------------------------------------------------
bool ECS::CheckComponentFromString(const std::string & name, Scene * scene, Entity entity)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.find(name);
	if (it != reg.end())
	{
		detail::CreateComponentFunc func = it->second;
		return func(scene, entity, detail::ComponentAction::Check);
	}

	//Check for user components
	DynamicDetail::DynamicComponentRegistry& d_reg = DynamicDetail::GetDynamicComponentRegistry();
	DynamicDetail::DynamicComponentRegistry::iterator d_it = d_reg.find(name);
	if (d_it != d_reg.end())
	{
		DynamicDetail::DynamicComponentFunc func = d_it->second;
		return func(scene, entity, detail::ComponentAction::Check);
	}

	return false;
}

bool ECS::DisplayComponentParameters(const std::string& name, Scene* scene, Entity entity)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.find(name);
	if (it != reg.end())
	{
		detail::CreateComponentFunc func = it->second;
		return func(scene, entity, detail::ComponentAction::Display);
	}


	//Check for user components
	DynamicDetail::DynamicComponentRegistry& d_reg = DynamicDetail::GetDynamicComponentRegistry();
	DynamicDetail::DynamicComponentRegistry::iterator d_it = d_reg.find(name);
	if (d_it != d_reg.end())
	{
		DynamicDetail::DynamicComponentFunc func = d_it->second;
		return func(scene, entity, detail::ComponentAction::Display);
	}
}

bool ECS::SerializeComponent(const std::string & name, Scene * scene, Entity entity)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.find(name);
	if (it != reg.end())
	{
		detail::CreateComponentFunc func = it->second;
		return func(scene, entity, detail::ComponentAction::Save);
	}


	//Check for user components
	DynamicDetail::DynamicComponentRegistry& d_reg = DynamicDetail::GetDynamicComponentRegistry();
	DynamicDetail::DynamicComponentRegistry::iterator d_it = d_reg.find(name);
	if (d_it != d_reg.end())
	{
		DynamicDetail::DynamicComponentFunc func = d_it->second;
		return func(scene, entity, detail::ComponentAction::Save);
	}
}

bool ECS::UnSerializeComponent(const std::string & name, Scene * scene, Entity entity)
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	detail::ComponentRegistry::iterator it = reg.find(name);
	if (it != reg.end())
	{
		detail::CreateComponentFunc func = it->second;
		return func(scene, entity, detail::ComponentAction::Load);
	}


	//Check for user components
	DynamicDetail::DynamicComponentRegistry& d_reg = DynamicDetail::GetDynamicComponentRegistry();
	DynamicDetail::DynamicComponentRegistry::iterator d_it = d_reg.find(name);
	if (d_it != d_reg.end())
	{
		DynamicDetail::DynamicComponentFunc func = d_it->second;
		return func(scene, entity, detail::ComponentAction::Load);
	}
}

unsigned int ECS::GetRegisteredComponentNumber()
{
	detail::ComponentRegistry& reg = detail::GetComponentRegistry();
	DynamicDetail::DynamicComponentRegistry d_reg = DynamicDetail::GetDynamicComponentRegistry();

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